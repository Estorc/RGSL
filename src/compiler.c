#include <RGSL/glsl/compile.h>
#include <RGSL/rgsl/compile.h>
#include <RGSL/compile.h>
#include <RGSL/fileio.h>
#include <RGSL/termio.h>
#include <RGSL/external/glslang_c.h>
#include <string.h>
#include <stdlib.h>

const struct language_compiler_mapping LANGUAGE_COMPILERS[] = {
    {"glsl", &rgsl_glsl_compile_shader},
    {"rgsl", &rgsl_rgsl_compile_shader}
};

bool (* const rgsl_select_language_compiler(const char* language))(struct shader_data *, char**) {
    size_t num_languages = sizeof(LANGUAGE_COMPILERS) / sizeof(LANGUAGE_COMPILERS[0]);
    for (size_t i = 0; i < num_languages; i++) {
        if (strcmp(language, LANGUAGE_COMPILERS[i].language) == 0) {
            return LANGUAGE_COMPILERS[i].compiler_func;
        }
    }
    return NULL;
}

bool rgsl_compile_shader(struct shader_data *shader, const char* output_file) {
    bool success = true;
    char * output = NULL;
    rgsl_printf_info(1, "Shader language detected: %s\n", shader->language);
    bool (*compiler_func)(struct shader_data *, char**) = rgsl_select_language_compiler(shader->language);
    size_t spv_size = 0;
    if (compiler_func != NULL) {
        success &= compiler_func(shader, &output);
        if (rgsl_global_options.action & RGSL_ACTION_COMPILE_SPIRV) {
            struct glslang_result glslang_result = glslang_compile_glsl(output, shader->stage);
            rgsl_free_file_buffer(output);
            output = NULL;
            if (glslang_result.success) {
                spv_size = glslang_result.word_count * sizeof(uint32_t);
                output = (char *)malloc(spv_size);
                memcpy(output, glslang_result.words, spv_size);
            } else {
                rgsl_printf_error("GLSL to SPIR-V compilation failed:\n%s\n", glslang_result.log);
                success = false;
            }
            glslang_free_result(&glslang_result);
        }
        if (success) {
            if (rgsl_global_options.action & RGSL_ACTION_COMPILE_EMBED) {
                rgsl_free_file_buffer(shader->code);
                shader->code = output;
                shader->word_count = spv_size / sizeof(uint32_t);
            } else {
                if (rgsl_write_file(output_file, output, spv_size)) {
                    rgsl_printf_info(1, "Compiled shader written to %s\n", output_file);
                } else {
                    rgsl_printf_error("Failed to open output file: %s\n", output_file);
                    success = false;
                }
                rgsl_free_file_buffer(output);
            }
        }
    } else {
        rgsl_printf_error("No compiler found for language: %s\n", shader->language);
        return false;
    }
    return success;
}