#include <RGSL/termio.h>
#include <RGSL/rgsl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rgsl_options rgsl_global_options = {};

static const struct language_mapping LANGUAGE_MAPPINGS[] = {
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

static const struct stage_mapping STAGE_MAPPINGS[] = {
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

void rgsl_print_version() {
    rgsl_print_info(0, "RGSL - RAE Graphics Shader Language Compiler\n");
    rgsl_print_info(0, "Version: 1.0.0\n");
    rgsl_print_info(0, "Copyright (c) 2024 RAE Software\n");
    rgsl_print_info(0, "Released under the MIT License\n");
}

void rgsl_initialize() {
    rgsl_global_options.input_files = NULL;
    rgsl_global_options.output_file = NULL;
    rgsl_global_options.include_paths = malloc(sizeof(char*) * 2);
    rgsl_global_options.include_paths[0] = ".";
    rgsl_global_options.include_paths[1] = NULL;
    rgsl_global_options.action = RGSL_ACTION_NONE;
    rgsl_global_options.show_version = false;
    rgsl_global_options.verbose = 1;
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

const char* rgsl_determine_shader_name(const char* filename) {
    const char* base = strrchr(filename, '/');
    if (base < strrchr(filename, '\\')) {
        base = strrchr(filename, '\\');
    }
    if (base != NULL) {
        base++; // Move past the slash
    } else {
        base = filename; // No path, use the whole filename
    }
    const char* dot = strrchr(base, '.');
    size_t name_length;
    if (dot != NULL) {
        name_length = (size_t)(dot - base);
    } else {
        name_length = strlen(base);
    }
    char* name = (char *)malloc((name_length + 1) * sizeof(char));
    strncpy_s(name, name_length + 1, base, name_length);
    name[name_length] = '\0';
    return name;
}