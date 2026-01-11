#include <RGSL/validator.h>
#include <RGSL/glsl/validator.h>
#include <RGSL/glsl/parser.h>
#include <RGSL/termio.h>
#include <RGSL/fileio.h>
#include <RGSL/parser.h>
#include <RGSL/rgsl.h>
#include <RGSL/external/glslang_c.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

bool rgsl_glsl_validate_shader(struct rgsl_shader_data * shader) {
    bool valid = true;
    rgsl_print_info(1, "Preprocessing GLSL shader code...\n");
    char* processed_code = rgsl_parse_shader(GLSL_DIRECTIVE_MAPPINGS, shader);
    if (processed_code == NULL) {
        rgsl_print_error("Failed to preprocess GLSL shader code.\n");
        return false;
    }

    rgsl_print_info(1, "Validating GLSL shader code with glslang...\n");
    char *log = NULL;
    valid &= rgsl_glslang_validate_glsl(processed_code, shader->stage, &log);

    if (!valid) {
        rgsl_printf_error("GLSL Validation Errors:\n%s\n", log);
    } else {
        rgsl_print_info(1, "GLSL shader code is valid.\n");
    }
    free(log);

    free(processed_code);
    return valid;
}