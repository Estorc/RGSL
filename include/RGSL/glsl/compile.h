/** ********************************************************************************
 * @section GLSL_Compile_Overview Overview
 * @file compile.h
 * @brief Header file for shader compilation functions.
 * @details
 * Typical use cases:
 * - Compiling GLSL shader code snippets.
 * *********************************************************************************
 * @section GLSL_Compile_Header Header
 * <RGSL/glsl/compile.h>
 ***********************************************************************************
 * @section GLSL_Compile_Metadata Metadata
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
#include <RGSL/compile.h>

/**
 * @brief Compiles the given shader data into the desired output format.
 * @param shader The shader data to compile.
 * @param output The code of the compiled shader.
 * @return true if compilation was successful, false otherwise.
 * 
 * This function compiles the provided shader code and writes the compiled output.
 */
bool rgsl_glsl_compile_shader(struct shader_data * shader, char** output);