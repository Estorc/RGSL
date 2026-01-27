/** ********************************************************************************
 * @section RGSL_Overview Overview
 * @file rgsl.h
 * @brief RGSL Command-Line Options Header
 * @details
 * Typical use cases:
 * - Managing RGSL command-line options.
 * *********************************************************************************
 * @section RGSL_Header Header
 * <RGSL/rgsl.h>
 ***********************************************************************************
 * @section RGSL_Metadata Metadata
 * @author Estorc
 * @version v1.0
 * @copyright Copyright (c) 2025 Estorc MIT License.
 **********************************************************************************/
/*                             This file is part of
 *                                     RGSL
 *                       (https://github.com/Estorc/RGSL)
 ***********************************************************************************
 * Copyright (c) 2025 Estorc.
 * This file is licensed under the MIT License.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************************/

#pragma once
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Structure to hold shader profile information.
 * 
 * This structure contains the version and profile name of a shader.
 */
struct rgsl_shader_profile {
    int version;
    const char* name;
};

/**
 * @brief Structure to hold shader data.
 * 
 * This structure contains information about a shader, including its name,
 * source code, word count, language, stage, and profile.
 */
struct rgsl_shader_data {
    const char* name;
    char* code;
    size_t word_count;
    const char* language;
    const char* stage;
    struct rgsl_shader_profile profile;
};

/**
 * @brief Structure to map file extensions to shader languages.
 * 
 * This structure holds a file extension and its corresponding shader language.
 */
struct rgsl_language_mapping {
    const char* extension;
    const char* language;
};

/**
 * @brief Structure to map file extensions to shader stages.
 * 
 * This structure holds a file extension and its corresponding shader stage.
 */
struct rgsl_stage_mapping {
    const char* extension;
    const char* stage;
};

/**
 * @brief Enumeration of RGSL command-line actions
 * 
 * This enumeration defines the possible actions that can be specified
 * via command-line arguments when running the RGSL tool.
 */
enum rgsl_action {
    RGSL_ACTION_NONE = 0,
    RGSL_ACTION_VALIDATE = 1 << 0,
    RGSL_ACTION_COMPILE = 1 << 1,
    RGSL_ACTION_COMPILE_EMBED = 1 << 2,
    RGSL_ACTION_COMPILE_SPIRV = 1 << 3
};

/**
 * @brief Structure to hold RGSL command-line options
 * 
 * This structure contains the various options that can be set via
 * command-line arguments when running the RGSL tool.
 */
struct rgsl_options {
    const char** input_files;
    const char* output_file;
    const char** include_paths;
    enum rgsl_action action;
    bool show_version;
    int verbose;
};

/**
 * @brief Global instance of RGSL command-line options
 * 
 * This variable holds the parsed command-line options for use
 * throughout the RGSL application.
 */
extern struct rgsl_options rgsl_global_options;

/**
 * @brief Initializes the RGSL global options with default values.
 * 
 * This function sets up the rgsl_global_options structure with default values,
 * including setting the input and output files to NULL, initializing include paths,
 * and setting the default action and verbosity level.
 */
void rgsl_initialize();

/**
 * @brief Prints the RGSL version information to the standard output.
 * 
 * This function outputs the RGSL version, copyright information, and license details
 * to the console.
 */
void rgsl_print_version();

/**
 * @brief Determines the shader stage based on the filename extension.
 * @param filename The name of the shader file.
 * @return The determined shader stage as a string (e.g., "vert", "frag").
 * 
 * This function analyzes the file extension of the provided filename to determine
 * the shader stage. Common extensions include .vert for vertex shaders and .frag
 * for fragment shaders.
 */
const char* rgsl_determine_shader_stage(const char* filename);

/**
 * @brief Determines the shader language based on the filename extension.
 * @param filename The name of the shader file.
 * @return The determined shader language as a string (e.g., "glsl", "rgsl").
 * 
 * This function analyzes the file extension of the provided filename to determine
 * the shader language. Common extensions include .glsl for GLSL shaders and .rgsl
 * for RGSL shaders.
 */
const char* rgsl_determine_shader_language(const char* filename);

/**
 * @brief Determines the shader name from the filename.
 * @param filename The name of the shader file.
 * @return The determined shader name as a string.
 * 
 * This function extracts the base name of the shader file (without path and extension)
 * to be used as the shader name.
 */
const char* rgsl_determine_shader_name(const char* filename);