#include <RGSL/parser.h>
#include <RGSL/termio.h>
#include <stdlib.h>
#include <string.h>

bool rgsl_read_preprocessor_directives(/* in */ const char* line,
                                      /* out */ struct directive* directive) {
    const size_t DIRECTIVE_NAME_SIZE = 64;
    const size_t DIRECTIVE_VALUE_SIZE = 256;                               
    if (directive == NULL) {
        return false;
    }
    directive->name = (char *)malloc(DIRECTIVE_NAME_SIZE * sizeof(char));
    directive->value = (char *)malloc(DIRECTIVE_VALUE_SIZE * sizeof(char));
    if (line[0] == '#') {
        int i = 1;
        while (line[i] != ' ' && line[i] != '\0') {
            directive->name[i - 1] = line[i];
            i++;
        }
        directive->name[i - 1] = '\0';
        while (line[i] == ' ') {
            i++;
        }
        int j = 0;
        while (line[i] != '\n' && line[i] != '\0') {
            directive->value[j++] = line[i++];
        }
        directive->value[j] = '\0';
    } else {
        return false;
    }
    return true;
}

void rgsl_free_directive(struct directive* directive) {
    if (directive != NULL) {
        free(directive->name);
        free(directive->value);
    }
}

bool rgsl_process_directive(const struct directive_mapping DIRECTIVE_MAPPINGS[], const struct directive directive, struct parser_state* state) {
    for (size_t i = 0; DIRECTIVE_MAPPINGS[i].directive != NULL; i++) {
        if (strcmp(directive.name, DIRECTIVE_MAPPINGS[i].directive) == 0) {
            char * replaced_line = NULL;
            int result = DIRECTIVE_MAPPINGS[i].handler_func(state, directive.value, &replaced_line);
            if (result != 0) {
                rgsl_printf_error("Error processing directive %s with value %s\n", directive.name, directive.value);
                return false;
            }
            if (replaced_line != NULL) {
                size_t replaced_length = strlen(replaced_line);
                size_t original_length = state->line_end - state->current_line;
                size_t line_offset = state->current_line - state->processed_code;
                size_t line_end_offset = state->line_end - state->processed_code;
                if (replaced_length <= original_length) {
                    memcpy(state->current_line, replaced_line, replaced_length);
                    // Fill the rest with spaces to maintain line length
                    memset(state->current_line + replaced_length, ' ', original_length - replaced_length);
                } else {
                    size_t tail_length = strlen(state->line_end);
                    state->processed_code = (char *)realloc(state->processed_code, strlen(state->processed_code) + (replaced_length - original_length) + 1);
                    state->current_line = state->processed_code + line_offset;
                    state->line_end = state->processed_code + line_end_offset;
                    memmove(state->current_line + replaced_length, state->line_end, tail_length + 1);
                    memcpy(state->current_line, replaced_line, replaced_length);
                }
                // This make like the line never existed so it can be reprocessed.
                state->line_end = state->current_line - 1;
                free(replaced_line);
            }
            return true;
        }
    }
    return true;
}

char * rgsl_parse_shader(const struct directive_mapping DIRECTIVE_MAPPINGS[], const char* shader_code) {
    struct parser_state state;
    state.processed_code = _strdup(shader_code);
    state.current_line = state.processed_code;
    state.version_directive_found = false;
    while (*state.current_line != '\0') {
        state.line_end = strchr(state.current_line, '\n');
        if (state.line_end == NULL) {
            state.line_end = state.current_line + strlen(state.current_line);
        }
        // Here you can add preprocessing logic, e.g., handling #include directives
        struct directive directive;
        bool found_directive = rgsl_read_preprocessor_directives(state.current_line, &directive);
        if (found_directive) {
            rgsl_process_directive(DIRECTIVE_MAPPINGS, directive, &state);
        }
        rgsl_free_directive(&directive);
        
        state.current_line = (*state.line_end == '\0') ? state.line_end : state.line_end + 1;
    }
    return state.processed_code;
}