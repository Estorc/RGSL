#include <RGSL/glsl/parser.h>
#include <RGSL/fileio.h>
#include <RGSL/termio.h>
#include <RGSL/rgsl.h>
#include <stdlib.h>
#include <string.h>

int rgsl_glsl_handle_include_directive(struct rgsl_parser_state* state, const char* value, void* out) {
    // Placeholder for handling #include directive
    rgsl_printf_info(2, "Handling #include directive with value: %s\n", value);
    if (out != NULL) {
        char **replaced_line = (char **)out;
        if (value[0] == '<') {
            // System include
            size_t rel_size = strlen(value) - 1 - strlen(strchr(value, '>'));
            for (size_t i = 0; rgsl_global_options.include_paths[i] != NULL; i++) {
                size_t len = strlen(rgsl_global_options.include_paths[i]) + rel_size + 2;
                char *possible_path = (char *)malloc(len);
                snprintf(possible_path, len, "%s/%s", rgsl_global_options.include_paths[i], value + 1);
                if (rgsl_file_exists(possible_path)) {
                    char *file_content;
                    rgsl_read_file(possible_path, &file_content);
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

int rgsl_glsl_handle_version_directive(struct rgsl_parser_state* state, const char* value, void* out) {
    // Placeholder for handling #version directive
    rgsl_printf_info(2, "Handling #version directive with value: %s\n", value);
    if (!state->version_directive_found) {
        state->shader->profile.version = atoi(value);
        const char* profile_start = strchr(value, ' ');
        if (profile_start != NULL) {
            char * profile_name = _strdup(profile_start + 1);
            profile_name[strcspn(profile_name, "\n")] = '\0'; // Remove newline
            state->shader->profile.name = profile_name;
        } else {
            state->shader->profile.name = "core"; // Default profile
        }
        state->version_directive_found = true;
    } else {
        char **replaced_line = (char **)out;
        *replaced_line = malloc(1 * sizeof(char));
        (*replaced_line)[0] = '\0'; // Remove duplicate version directive
    }
    return 0; // Success
}

const struct rgsl_directive_mapping GLSL_DIRECTIVE_MAPPINGS[] = {
    {"include", rgsl_glsl_handle_include_directive},
    {"version", rgsl_glsl_handle_version_directive},
    {NULL, NULL} // Sentinel to mark the end of the array
};