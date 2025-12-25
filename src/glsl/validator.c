#include <RGSL/validator.h>
#include <RGSL/glsl/validator.h>
#include <RGSL/termio.h>
#include <RGSL/fileio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

const char* search_paths[] = {
    "examples/raeptor_cogs/"
};

int rgsl_glsl_handle_include_directive(const char* value, void* out) {
    // Placeholder for handling #include directive
    rgsl_printf_info("Handling #include directive with value: %s\n", value);
    if (out != NULL) {
        char **replaced_line = (char **)out;
        if (value[0] == '<') {
            // System include
            size_t rel_size = strlen(value) - 1 - strlen(strchr(value, '>'));
            size_t path_count = sizeof(search_paths) / sizeof(search_paths[0]);
            for (size_t i = 0; i < path_count; i++) {
                size_t len = strlen(search_paths[i]) + rel_size + 1;
                char *possible_path = (char *)malloc(len);
                snprintf(possible_path, len, "%s%s", search_paths[i], value + 1);
                if (rgsl_file_exists(possible_path)) {
                    char *file_content = rgsl_read_file(possible_path);
                    if (file_content != NULL) {
                        *replaced_line = file_content;
                        free(possible_path);
                        return 0; // Success
                    }
                }
                free(possible_path);
            }
            rgsl_printf_error("Included file %s not found in search paths.\n", value);
            return -1; // File not found
        } else if (value[0] == '\"') {
            // Local include
            rgsl_print_error("Local includes not yet implemented.\n");
            return -1; // Not implemented
        }
    }
    return 0; // Success
}

const struct directive_mapping DIRECTIVE_MAPPINGS[] = {
    {"include", rgsl_glsl_handle_include_directive},
};

bool rgsl_glsl_process_directive(char** processed_code, char** line, char** line_end, const char* directive, const char* value) {
    size_t num_directives = sizeof(DIRECTIVE_MAPPINGS) / sizeof(DIRECTIVE_MAPPINGS[0]);
    for (size_t i = 0; i < num_directives; i++) {
        if (strcmp(directive, DIRECTIVE_MAPPINGS[i].directive) == 0) {
            char * replaced_line = NULL;
            int result = DIRECTIVE_MAPPINGS[i].handler_func(value, &replaced_line);
            if (result != 0) {
                rgsl_printf_error("Error processing directive %s with value %s\n", directive, value);
                return false;
            }
            if (replaced_line != NULL) {
                size_t replaced_length = strlen(replaced_line);
                size_t original_length = *line_end - *line;
                size_t line_offset = *line - *processed_code;
                size_t line_end_offset = *line_end - *processed_code;
                if (replaced_length <= original_length) {
                    memcpy(*line, replaced_line, replaced_length);
                    // Fill the rest with spaces to maintain line length
                    memset(*line + replaced_length, ' ', original_length - replaced_length);
                } else {
                    size_t tail_length = strlen(*line_end);
                    *processed_code = (char *)realloc(*processed_code, strlen(*processed_code) + (replaced_length - original_length) + 1);
                    *line = *processed_code + line_offset;
                    *line_end = *processed_code + line_end_offset;
                    memmove(*line + replaced_length, *line_end, tail_length + 1);
                    memcpy(*line, replaced_line, replaced_length);
                }
                // This make like the line never existed so it can be reprocessed.
                *line_end = *line - 1;
                free(replaced_line);
            }
            return true;
        }
    }
    return true;
}

char * rgsl_glsl_preprocess_shader(const char* shader_code) {
    char *processed_code = _strdup(shader_code);
    char *line = processed_code;
    while (*line != '\0') {
        char *line_end = strchr(line, '\n');
        if (line_end == NULL) {
            line_end = line + strlen(line);
        }
        // Here you can add preprocessing logic, e.g., handling #include directives
        char *directive_name = NULL;
        char *directive_value = NULL;
        bool found_directive = rgsl_read_preprocessor_directives(line, &directive_name, &directive_value);
        if (found_directive) {
            rgsl_glsl_process_directive(&processed_code, &line, &line_end, directive_name, directive_value);
        }
        free(directive_name);
        free(directive_value);
        
        line = (*line_end == '\0') ? line_end : line_end + 1;
    }
    return processed_code;
}

bool rgsl_glsl_validate_shader(struct shader_data shader) {
    bool valid = true;
    rgsl_print_info("Preprocessing GLSL shader code...\n");
    char* processed_code = rgsl_glsl_preprocess_shader(shader.code);
    if (processed_code == NULL) {
        rgsl_print_error("Failed to preprocess GLSL shader code.\n");
        return false;
    }

    FILE *f;
    fopen_s(&f, ".__rgsl_tmp.glsl", "wb");
    fputs(processed_code, f);
    fclose(f);

    rgsl_print_info("Validating GLSL shader code with glslangValidator...\n");
    char command[256];
    snprintf(command, sizeof(command), "glslangValidator -S %s .__rgsl_tmp.glsl > errors.txt 2>&1", shader.stage);
    valid &= !system(command);

    if (!valid) {
        char *error_log = rgsl_read_file("errors.txt");
        if (error_log != NULL) {
            rgsl_printf_error("GLSL Validation Errors:\n%s\n", error_log);
            free(error_log);
        } else {
            rgsl_print_error("Failed to read GLSL validation error log.\n");
        }
    } else {
        rgsl_print_info("GLSL shader code is valid.\n");
    }
    remove(".__rgsl_tmp.glsl");
    remove("errors.txt");

    free(processed_code);
    return valid;
}