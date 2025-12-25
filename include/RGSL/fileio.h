/** ********************************************************************************
 * @section Fileio_Overview Overview
 * @file fileio.h
 * @brief Header file for file input/output functions.
 * @details
 * Typical use cases:
 * - Reading shader code from files.
 * *********************************************************************************
 * @section Fileio_Header Header
 * <RGSL/fileio.h>
 ***********************************************************************************
 * @section Fileio_Metadata Metadata
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
 * @brief Reads the entire content of a file into a dynamically allocated string.
 * @param filename The path to the file to read.
 * @return A pointer to the dynamically allocated string containing the file content,
 * or NULL if the file could not be opened or read.
 * 
 * This function opens the specified file in binary mode, reads its entire content,
 * and returns it as a null-terminated string. The caller is responsible for freeing
 * the allocated memory.
 * 
 * @code{c}
 * char *shader_code = read_file("shader.glsl");
 * if (shader_code != NULL) {
 *     // Use shader_code...
 *    free(shader_code);
 * }
 * @endcode
 */
char *rgsl_read_file(const char* filename);

/**
 * @brief Converts all CRLF line endings in a string to LF line endings.
 * @param str The input string with potential CRLF line endings.
 * @return A pointer to a newly allocated string with LF line endings,
 * or NULL if memory allocation fails.
 * 
 * This function scans the input string for CRLF ("\r\n") sequences and
 * replaces them with LF ("\n") line endings. The resulting string is
 * dynamically allocated, and the caller is responsible for freeing it.
 * 
 * @code{c}
 * char *converted_str = rgsl_crlf_to_lf(original_str);
 * if (converted_str != NULL) {
 *     // Use converted_str...
 *    free(converted_str);
 * }
 * @endcode
 */
char *rgsl_crlf_to_lf(const char* str);

/**
 * @brief Frees the memory allocated for a file buffer.
 * @param buffer The pointer to the buffer to free.
 * 
 * This function frees the memory allocated for a file buffer that was
 * previously obtained using the rgsl_read_file function.
 * 
 * @code{c}
 * char *shader_code = read_file("shader.glsl");
 * if (shader_code != NULL) {
 *     // Use shader_code...
 *     rgsl_free_file_buffer(shader_code);
 * }
 * @endcode
 */
void rgsl_free_file_buffer(char* buffer);

/**
 * @brief Checks if a file exists at the specified path.
 * @param filename The path to the file to check.
 * @return true if the file exists, false otherwise.
 * 
 * This function attempts to open the specified file in binary mode. If the file
 * can be opened, it is assumed to exist, and the function returns true. If the
 * file cannot be opened, the function returns false.
 */
bool rgsl_file_exists(const char* filename);
