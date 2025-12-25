/** ********************************************************************************
 * @section Validator_Overview Overview
 * @file validator.h
 * @brief Header file for shader validation functions.
 * @details
 * Typical use cases:
 * - Validating shader code snippets.
 * *********************************************************************************
 * @section Validator_Header Header
 * <RGSL/validator.h>
 ***********************************************************************************
 * @section Validator_Metadata Metadata
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

/**
 * @brief Structure to hold shader data.
 * 
 * This structure contains the shader code and its associated stage (e.g., vert, frag).
 */
struct shader_data {
    char* code;
    const char* language;
    const char* stage;
};

/**
 * @brief Structure to map file extensions to shader languages.
 * 
 * This structure holds a file extension and its corresponding shader language.
 */
struct language_mapping {
    const char* extension;
    const char* language;
};

/**
 * @brief Structure to map shader languages to their corresponding validator functions.
 * 
 * This structure holds the name of a shader language and a pointer to the function
 * that validates shaders written in that language.
 */
struct language_validator_mapping {
    const char* language;
    bool (*validator_func)(struct shader_data);
};

/**
 * @brief Structure to map preprocessor directives to their handler functions.
 * 
 * This structure holds the name of a preprocessor directive and a pointer to the function
 * that handles that directive.
 */
struct directive_mapping {
    const char* directive;
    int (*handler_func)(const char* value, void* out);
};

/**
 * @brief Structure to map file extensions to shader stages.
 * 
 * This structure holds a file extension and its corresponding shader stage.
 */
struct stage_mapping {
    const char* extension;
    const char* stage;
};

/**
 * @brief Validates the given shader code.
 * @param shader The shader data to validate.
 * @return true if the shader code is valid, false otherwise.
 * 
 * This function performs a basic validation of the provided shader code.
 * Currently, it checks if the code is non-empty. More comprehensive validation
 * can be added in the future.
 */
bool rgsl_validate_shader(struct shader_data shader);

/**
 * @brief Checks for preprocessor directives in the given line of shader code.
 * @param line The line of shader code to check.
 * @param directive_name Output parameter to store the name of the directive.
 * @param directive_value Output parameter to store the value of the directive.
 * @return true if a preprocessor directive is found, false otherwise.
 * 
 * This function scans the provided line of shader code for preprocessor directives
 * (e.g., #version, #include). If a directive is found, it extracts the name and value
 * and stores them in the provided output parameters.
 */
bool rgsl_read_preprocessor_directives(/* in */ const char* line,
                                      /* out */ char** directive_name,
                                      /* out */ char** directive_value);

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