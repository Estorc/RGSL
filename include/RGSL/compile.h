/** ********************************************************************************
 * @section Compile_Overview Overview
 * @file compile.h
 * @brief Header file for shader compilation functions.
 * @details
 * Typical use cases:
 * - Compiling shader code snippets.
 * *********************************************************************************
 * @section Compile_Header Header
 * <RGSL/compile.h>
 ***********************************************************************************
 * @section Compile_Metadata Metadata
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
 * @brief Structure to map shader languages to their compiler functions.
 * 
 * This structure holds the name of a shader language and a pointer to the function
 * that compiles shaders written in that language.
 */
struct language_compiler_mapping {
    const char* language;
    bool (*compiler_func)(struct shader_data *, char**);
};

/**
 * @brief Compiles the given shader data into the desired output format.
 * @param shader The shader data to compile.
 * @param output_file The path to the output file where the compiled shader will be saved.
 * @return true if compilation was successful, false otherwise.
 * 
 * This function compiles the provided shader code and writes the compiled output
 * to the specified output file. The compilation process may vary depending on
 * the shader language and stage.
 */
bool rgsl_compile_shader(struct shader_data * shader, const char* output_file);