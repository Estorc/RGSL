/** ********************************************************************************
 * @section Glslang_C_Overview Overview
 * @file glslang_c.h
 * @brief Header file for GLSL shader compilation and validation using glslang.
 * @details
 * Typical use cases:
 * - Validating GLSL shader code snippets.
 * - Compiling GLSL shader code snippets to SPIR-V.
 * *********************************************************************************
 * @section Glslang_C_Header Header
 * <RGSL/external/glslang_c.h>
 ***********************************************************************************
 * @section Glslang_C_Metadata Metadata
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
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Structure to hold the result of GLSL compilation.
 * 
 * This structure contains the compiled SPIR-V words, the number of words,
 * the compilation log, and a success flag.
 */
struct rgsl_glslang_result {
    const uint32_t* words;
    size_t word_count;
    const char* log;
    int success;
};

/**
 * @brief Initializes the glslang process.
 * 
 * This function should be called before any other glslang functions are used.
 */
void rgsl_glslang_initialize();

/**
 * @brief Finalizes the glslang process.
 * 
 * This function should be called to clean up resources used by glslang after
 * all glslang functions have been used.
 */
void rgsl_glslang_finalize();

/**
 * @brief Validates GLSL shader source code.
 * @param source The GLSL shader source code as a null-terminated string.
 * @param stage_str The shader stage as a string (e.g., "vert", "frag").
 * @param out_log Pointer to a char pointer that will receive the validation log.
 * @return true if the shader code is valid, false otherwise.
 * 
 * This function uses the glslang library to validate the provided GLSL shader code
 * for the specified shader stage. The validation log is returned via the out_log
 * parameter, which should be freed by the caller if not NULL.
 * 
 * @note The caller is responsible for freeing the out_log buffer.
 */
bool rgsl_glslang_validate_glsl(const char* source, const char* stage_str, char** out_log);

/**
 * @brief Compiles GLSL shader source code to SPIR-V.
 * @param source The GLSL shader source code as a null-terminated string.
 * @param stage The shader stage as a string (e.g., "vert", "frag").
 * @return A glslang_result_t structure containing the compilation result,
 * including SPIR-V words, word count, log, and success status.
 * 
 * This function compiles the provided GLSL shader code for the specified shader stage
 * into SPIR-V binary format. The resulting SPIR-V words and compilation log are
 * included in the returned rgsl_glslang_result structure. The caller is responsible for freeing
 * the log and words in the rgsl_glslang_result structure using rgsl_glslang_free_result.
 */
struct rgsl_glslang_result rgsl_glslang_compile_glsl(const char* source, const char* stage);

/**
 * @brief Frees the resources allocated in a rgsl_glslang_result structure.
 * @param r Pointer to the rgsl_glslang_result structure to free.
 * 
 * This function frees the memory allocated for the log and words in the
 * provided rgsl_glslang_result structure.
 */
void rgsl_glslang_free_result(struct rgsl_glslang_result* r);

#ifdef __cplusplus
}
#endif
