#include <RGSL/validator.h>
#include <RGSL/compile.h>
#include <RGSL/packager.h>
#include <RGSL/termio.h>
#include <RGSL/fileio.h>
#include <RGSL/rgsl.h>
#include <RGSL/external/glslang_c.h>
#include <argparse/argparse.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#define _strdup strdup
#endif

static int on_include_option(struct argparse *self, const struct argparse_option *) {
    static int count = 2;
    
    const char *value;
    if (self->optvalue) {
        value = self->optvalue;
        self->optvalue = NULL;
    } else if (self->argc > 1) {
        self->argc--;
        value = *++self->argv;
    } else {
        rgsl_print_error("The --include option requires a value\n");
        return -1;
    }

    if (rgsl_global_options.include_paths == NULL) {
        rgsl_global_options.include_paths = (const char**)malloc(sizeof(char*) * 2);
        rgsl_global_options.include_paths[0] = NULL;
    }

    rgsl_global_options.include_paths = realloc(rgsl_global_options.include_paths, sizeof(char*) * (count + 1));
    rgsl_global_options.include_paths[count++] = NULL;
    rgsl_global_options.include_paths[count - 2] = _strdup(value);

    return 0;
}

int main(int argc, const char** argv) {
    rgsl_initialize();
    struct rgsl_shader_data* shaders = NULL;
    struct argparse_option options[] = {
        OPT_GROUP("File options"),
        OPT_STRING('o', "output", &rgsl_global_options.output_file, "output file"),
        OPT_STRING('I', "include", NULL, "additional include paths", on_include_option),
        OPT_GROUP("Action options (choose at least one)"),
        OPT_BIT('V', "validate", &rgsl_global_options.action, "validate the input shader file", NULL, RGSL_ACTION_VALIDATE, 0),
        OPT_BIT('C', "compile", &rgsl_global_options.action, "compile the input shader file", NULL, RGSL_ACTION_COMPILE, 0),
        OPT_BIT('S', "spirv", &rgsl_global_options.action, "compile the input shader to SPIR-V", NULL, RGSL_ACTION_COMPILE_SPIRV, 0),
        OPT_BIT(0, "embed", &rgsl_global_options.action, "merge the input shaders to an embeddable C array", NULL, RGSL_ACTION_COMPILE_EMBED, 0),
        OPT_GROUP("Misc options"),
        OPT_HELP(),
        OPT_BOOLEAN('v', "version", &rgsl_global_options.show_version, "show version information and exit"),
        OPT_INTEGER(0, "verbose", &rgsl_global_options.verbose, "set verbosity level (0=quiet, 1=normal, 2=verbose)", NULL, 1),
        OPT_END(),
    };

    const char * const usages[] = {
        "rgsl [options] <input file>...",
        NULL,
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_parse(&argparse, argc, argv);

    if (argparse.out[0] != NULL) {
        rgsl_global_options.input_files = argparse.out;
    }

    if (rgsl_global_options.show_version) {
        rgsl_print_version();
        return 0;
    }

    if (!rgsl_global_options.input_files) {
        rgsl_print_error("No input file specified\n");
        argparse_usage(&argparse);
        return 1;
    }

    if (rgsl_global_options.action == RGSL_ACTION_NONE) {
        rgsl_print_error("At least one action (--validate or --compile) must be specified\n");
        argparse_usage(&argparse);
        return 1;
    }

    if (rgsl_global_options.action & RGSL_ACTION_COMPILE_EMBED) {
        size_t num_inputs;
        for (num_inputs = 0; rgsl_global_options.input_files[num_inputs] != NULL; num_inputs++);
        shaders = (struct rgsl_shader_data*)malloc(sizeof(struct rgsl_shader_data) * (num_inputs + 1));
        shaders[num_inputs] = (struct rgsl_shader_data){0};
    } else if (rgsl_global_options.input_files[1] != NULL) {
        rgsl_print_info(0, "Multiple input files detected. To embed multiple shaders into a single C array, use the --embed option.\n");
        rgsl_print_error("Only one input file can be processed at a time unless using --embed\n");
        argparse_usage(&argparse);
        return 1;
    }

    rgsl_glslang_initialize();
    for (int i = 0; rgsl_global_options.input_files[i] != NULL; i++) {
        const char* input_file = rgsl_global_options.input_files[i];
        rgsl_printf_info(3, "Input file: %s\n", input_file);

        if (!rgsl_file_exists(input_file)) {
            rgsl_printf_error("Input file does not exist: %s\n", input_file);
            return 1;
        }

        struct rgsl_shader_data shader;
        const char* shader_file = input_file;
        char* raw_shader_code;
        rgsl_read_file(shader_file, &raw_shader_code);
        shader.name = rgsl_determine_shader_name(shader_file);
        shader.code = rgsl_crlf_to_lf(raw_shader_code);
        shader.language = rgsl_determine_shader_language(shader_file);
        shader.stage = rgsl_determine_shader_stage(shader_file);
        rgsl_free_file_buffer(raw_shader_code);
        if (shader.name == NULL) {
            rgsl_printf_error("Could not determine shader name from file: %s\n", shader_file);
            return 1;
        }
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

        if (rgsl_global_options.action & RGSL_ACTION_VALIDATE) {
            bool is_valid = rgsl_validate_shader(&shader);
            if (is_valid) {
                rgsl_printf_info(1, "Shader %s is valid.\n", shader_file);
            } else {
                rgsl_printf_error("Shader %s is invalid.\n", shader_file);
                return 1;
            }
        }
        if (rgsl_global_options.action & RGSL_ACTION_COMPILE || rgsl_global_options.action & RGSL_ACTION_COMPILE_SPIRV) {
            if (!rgsl_global_options.output_file) {
                rgsl_print_error("Output file must be specified for compilation using --output\n");
                rgsl_free_file_buffer(shader.code);
                return 1;
            }
            rgsl_printf_info(1, "Compiling shader %s to %s...\n", shader_file, rgsl_global_options.output_file);
            rgsl_compile_shader(&shader, rgsl_global_options.output_file);
        }
        if (rgsl_global_options.action & RGSL_ACTION_COMPILE_EMBED) {
            shaders[i] = shader;
        } else {
            rgsl_free_file_buffer(shader.code);
        }
    }
    if (rgsl_global_options.action & RGSL_ACTION_COMPILE_EMBED) {
        rgsl_package_shaders(shaders);
        for (int i = 0; shaders[i].code != NULL; i++) {
            rgsl_free_file_buffer(shaders[i].code);
        }
        free(shaders);
    }
    rgsl_glslang_finalize();
    return 0;
}