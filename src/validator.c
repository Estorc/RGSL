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

const struct language_mapping LANGUAGE_MAPPINGS[] = {
    /** 
     * GLSL 
     */
    {".glsl", "glsl"},

    {".vert", "glsl"},
    {".vs", "glsl"},

    {".frag", "glsl"},
    {".fs", "glsl"},

    {".geom", "glsl"},
    {".gs", "glsl"},

    {".comp", "glsl"},
    {".cs", "glsl"},

    {".tesc", "glsl"},
    {".tc", "glsl"},

    {".tese", "glsl"},
    {".te", "glsl"},

    /** 
     * RGSL
     */
    {".rgsl", "rgsl"},

    {".rvert", "rgsl"},
    {".rvs", "rgsl"},

    {".rfrag", "rgsl"},
    {".rfs", "rgsl"},

    {".rgeom", "rgsl"},
    {".rgs", "rgsl"},

    {".rcomp", "rgsl"},
    {".rcs", "rgsl"},

    {".rtesc", "rgsl"},
    {".rtc", "rgsl"},

    {".rtese", "rgsl"},
    {".rte", "rgsl"}
};

const struct stage_mapping STAGE_MAPPINGS[] = {
    /** vertex shaders */
    {".vert", "vert"},
    {".vs", "vert"},

    /** fragment shaders */
    {".frag", "frag"},
    {".fs", "frag"},

    /** geometry shaders */
    {".geom", "geom"},
    {".gs", "geom"},

    /** compute shaders */
    {".comp", "comp"},
    {".cs", "comp"},

    /** tessellation control shaders */
    {".tesc", "tesc"},
    {".tc", "tesc"},

    /** tessellation evaluation shaders */
    {".tese", "tese"},
    {".te", "tese"},
};

bool rgsl_check_non_empty(const char* code) {
    bool result = code != NULL && code[0] != '\0';
    if (!result) {
        rgsl_print_error("Shader code is empty!\n");
    }
    return result;
}

bool rgsl_read_preprocessor_directives(/* in */ const char* line,
                                      /* out */ char** directive_name,
                                      /* out */ char** directive_value) {
    const size_t DIRECTIVE_NAME_SIZE = 64;
    const size_t DIRECTIVE_VALUE_SIZE = 256;                               
    if (directive_name == NULL || directive_value == NULL) {
        return false;
    }
    *directive_name = (char *)malloc(DIRECTIVE_NAME_SIZE * sizeof(char));
    *directive_value = (char *)malloc(DIRECTIVE_VALUE_SIZE * sizeof(char));
    if (line[0] == '#') {
        int i = 1;
        while (line[i] != ' ' && line[i] != '\0') {
            (*directive_name)[i - 1] = line[i];
            i++;
        }
        (*directive_name)[i - 1] = '\0';
        while (line[i] == ' ') {
            i++;
        }
        int j = 0;
        while (line[i] != '\n' && line[i] != '\0') {
            (*directive_value)[j++] = line[i++];
        }
        (*directive_value)[j] = '\0';
    } else {
        return false;
    }
    return true;
}

bool (* const rgsl_select_language_validator(const char* language))(struct shader_data) {
    size_t num_languages = sizeof(LANGUAGE_VALIDATORS) / sizeof(LANGUAGE_VALIDATORS[0]);
    for (size_t i = 0; i < num_languages; i++) {
        if (strcmp(language, LANGUAGE_VALIDATORS[i].language) == 0) {
            return LANGUAGE_VALIDATORS[i].validator_func;
        }
    }
    return NULL;
}

const char* rgsl_determine_shader_stage(const char* filename) {
    size_t num_mappings = sizeof(STAGE_MAPPINGS) / sizeof(STAGE_MAPPINGS[0]);
    const char* extension = strrchr(filename, '.');
    if (extension != NULL) {
        for (size_t i = 0; i < num_mappings; i++) {
            if (strcmp(extension, STAGE_MAPPINGS[i].extension) == 0) {
                return STAGE_MAPPINGS[i].stage;
            }
        }
    }
    return NULL;
}

const char* rgsl_determine_shader_language(const char* filename) {
    size_t num_mappings = sizeof(LANGUAGE_MAPPINGS) / sizeof(LANGUAGE_MAPPINGS[0]);
    const char* extension = strrchr(filename, '.');
    if (extension != NULL) {
        for (size_t i = 0; i < num_mappings; i++) {
            if (strcmp(extension, LANGUAGE_MAPPINGS[i].extension) == 0) {
                return LANGUAGE_MAPPINGS[i].language;
            }
        }
    }
    return NULL;
}

bool rgsl_validate_shader(struct shader_data shader) {
    bool is_valid = true;
    is_valid &= rgsl_check_non_empty(shader.code);
    rgsl_printf_info("Shader language detected: %s\n", shader.language);
    bool (*validator_func)(struct shader_data) = rgsl_select_language_validator(shader.language);
    if (validator_func != NULL) {
        is_valid &= validator_func(shader);
    } else {
        rgsl_printf_error("No validator found for language: %s\n", shader.language);
        is_valid = false;
    }
    return is_valid;
}
