#include <RGSL/glsl/compile.h>
#include <RGSL/glsl/parser.h>
#include <RGSL/termio.h>

bool rgsl_glsl_compile_shader(struct shader_data * shader, char** output) {
    rgsl_print_info(1, "Preprocessing GLSL shader code...\n");
    *output = rgsl_parse_shader(GLSL_DIRECTIVE_MAPPINGS, shader->code);
    if (*output == NULL) {
        rgsl_print_error("Failed to preprocess GLSL shader code.\n");
        return false;
    }
    return true;
}