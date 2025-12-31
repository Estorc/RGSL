/** ********************************************************************************
 * @section Parser_Overview Overview
 * @file parser.h
 * @brief Header file for shader parsing functions.
 * @details
 * Typical use cases:
 * - Parsing shader code.
 * *********************************************************************************
 * @section Parser_Header Header
 * <RGSL/parser.h>
 ***********************************************************************************
 * @section Parser_Metadata Metadata
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
#include <RGSL/rgsl.h>

/**
 * @brief Structure to maintain the state of the parser.
 * 
 * This structure holds information about the current state of the shader
 * parsing process, including the shader being processed, the processed code,
 * current line pointers, and flags for directive handling.
 */
struct parser_state {
    struct shader_data* shader;
    char* processed_code;
    char* current_line;
    char* line_end;
    bool version_directive_found;
};

/**
 * @brief Structure to map preprocessor directives to their handler functions.
 * 
 * This structure holds the name of a preprocessor directive and a pointer to the function
 * that handles that directive.
 */
struct directive_mapping {
    const char* directive;
    int (*handler_func)(struct parser_state*, const char* value, void* out);
};

/**
 * @brief Structure to hold a preprocessor directive's name and value.
 * 
 * This structure contains the name of the directive and its associated value.
 */
struct directive {
    char* name;
    char* value;
};

/**
 * @brief Checks for preprocessor directives in the given line of shader code.
 * @param line The line of shader code to check.
 * @param directive Output parameter to hold the found directive's name and value.
 * @return true if a preprocessor directive is found, false otherwise.
 * 
 * This function scans the provided line of shader code for preprocessor directives
 * (e.g., #version, #include). If a directive is found, it extracts the name and value
 * and stores them in the provided output parameters.
 */
bool rgsl_read_preprocessor_directives(/* in */ const char* line,
                                      /* out */ struct directive* directive);

/**
 * @brief Frees the memory allocated for a directive's name and value.
 * @param directive Pointer to the directive whose resources are to be freed.
 * 
 * This function releases the memory allocated for the name and value of the given directive.
 */
void rgsl_free_directive(struct directive* directive);

/**
 * @brief Processes a preprocessor directive found in the shader code.
 * @param DIRECTIVE_MAPPINGS An array of directive mappings to handle different directives.
 * @param directive The directive to process.
 * @param state The current state of the parser, including processed code and line pointers.
 * 
 * This function looks up the provided directive in the given directive mappings
 * and invokes the corresponding handler function. It modifies the processed code
 * and line pointers as necessary based on the directive's effect.
 */
bool rgsl_process_directive(const struct directive_mapping DIRECTIVE_MAPPINGS[], const struct directive directive, struct parser_state* state);

/**
 * @brief Parses the shader code, handling preprocessor directives.
 * @param DIRECTIVE_MAPPINGS An array of directive mappings to handle different directives.
 * @param shader_code The original shader code to parse.
 * @return A pointer to the processed shader code with directives handled.
 * 
 * This function processes the provided shader code line by line, checking for
 * preprocessor directives and applying the corresponding transformations based
 * on the provided directive mappings.
 * 
 * @note The returned string is dynamically allocated and should be freed by the caller.
 */
char * rgsl_parse_shader(const struct directive_mapping DIRECTIVE_MAPPINGS[], struct shader_data* shader);