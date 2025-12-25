#include <RGSL/validator.h>
#include <RGSL/termio.h>
#include <RGSL/fileio.h>

int main(int argc, char** argv) {
    rgsl_printf_info("RGSL Validator Main\n");
    if (argc < 2) {
        rgsl_print_error("Usage: rgsl_validator <shader_file>\n");
        return 1;
    }
    struct shader_data shader;
    const char* shader_file = argv[1];
    char* raw_shader_code = rgsl_read_file(shader_file);
    shader.code = rgsl_crlf_to_lf(raw_shader_code);
    shader.language = rgsl_determine_shader_language(shader_file);
    shader.stage = rgsl_determine_shader_stage(shader_file);
    rgsl_free_file_buffer(raw_shader_code);
    if (shader.code == NULL) {
        rgsl_printf_error("Failed to read shader file: %s\n", shader_file);
        return 1;
    }
    if (shader.language == NULL) {
        rgsl_printf_error("Could not determine shader language from file extension: %s\n", shader_file);
        rgsl_free_file_buffer(shader.code);
        return 1;
    }
    if (shader.stage == NULL) {
        rgsl_printf_error("Could not determine shader stage from file extension: %s\n", shader_file);
        rgsl_free_file_buffer(shader.code);
        return 1;
    }
    bool is_valid = rgsl_validate_shader(shader);
    if (is_valid) {
        rgsl_printf_info("Shader %s is valid.\n", shader_file);
    } else {
        rgsl_printf_error("Shader %s is invalid.\n", shader_file);
    }
    rgsl_free_file_buffer(shader.code);
    return 0;
}