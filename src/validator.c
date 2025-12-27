#include <RGSL/validator.h>
#include <RGSL/glsl/validator.h>
#include <RGSL/rgsl/validator.h>
#include <RGSL/termio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

const struct language_validator_mapping LANGUAGE_VALIDATORS[] = {
    {"glsl", &rgsl_glsl_validate_shader},
    {"rgsl", &rgsl_rgsl_validate_shader}
};

bool rgsl_check_non_empty(const char* code) {
    bool result = code != NULL && code[0] != '\0';
    if (!result) {
        rgsl_print_error("Shader code is empty!\n");
    }
    return result;
}

bool (* const rgsl_select_language_validator(const char* language))(struct shader_data *) {
    size_t num_languages = sizeof(LANGUAGE_VALIDATORS) / sizeof(LANGUAGE_VALIDATORS[0]);
    for (size_t i = 0; i < num_languages; i++) {
        if (strcmp(language, LANGUAGE_VALIDATORS[i].language) == 0) {
            return LANGUAGE_VALIDATORS[i].validator_func;
        }
    }
    return NULL;
}

bool rgsl_validate_shader(struct shader_data * shader) {
    bool is_valid = true;
    is_valid &= rgsl_check_non_empty(shader->code);
    rgsl_printf_info(1, "Shader language detected: %s\n", shader->language);
    bool (*validator_func)(struct shader_data *) = rgsl_select_language_validator(shader->language);
    if (validator_func != NULL) {
        is_valid &= validator_func(shader);
    } else {
        rgsl_printf_error("No validator found for language: %s\n", shader->language);
        is_valid = false;
    }
    return is_valid;
}
