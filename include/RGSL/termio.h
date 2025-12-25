/** ********************************************************************************
 * @section Termio_Overview Overview
 * @file termio.h
 * @brief Header file for terminal input/output functions.
 * @details
 * Typical use cases:
 * - Printing informational and error messages to the terminal.
 * *********************************************************************************
 * @section Termio_Header Header
 * <RGSL/termio.h>
 ***********************************************************************************
 * @section Termio_Metadata Metadata
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
#include <stdio.h>

/**
 * @brief Prints a formatted message to the specified output stream.
 * @param stream The output stream (e.g., stdout, stderr).
 * @param prefix The prefix to include in the message (e.g., "Info", "Error").
 * @param message The message to print.
 * 
 * This function prints a message prefixed with "[RGSL <prefix>]" to the given output stream.
 * It flushes the stream to ensure immediate output.
 * 
 * @code{c}
 * rgsl_fprint(stdout, "Info", "This is an informational message.");
 * rgsl_fprint(stderr, "Error", "This is an error message.");
 * @endcode
 */
void rgsl_fprint(FILE *stream, const char* prefix, const char* message);

/**
 * @brief Formats a message using a format string and variable arguments.
 * @param format The format string (printf-style).
 * @param args The variable argument list.
 * @param out_buffer Pointer to the output buffer that will hold the formatted message.
 * 
 * This function formats a message according to the specified format string and
 * variable arguments. The formatted message is stored in a dynamically allocated
 * buffer pointed to by out_buffer. The caller is responsible for freeing the buffer.
 * 
 * @code{c}
 * char* buffer = NULL;
 * va_list args;
 * va_start(args, format);
 * rgsl_format_parser("Value: %d", args, &buffer);
 * va_end(args);
 * // Use buffer...
 * free(buffer);
 * @endcode
 */
void rgsl_format_parser(const char* format, va_list args, char** out_buffer);

/**
 * @brief Prints a formatted message to the specified output stream using a format string.
 * @param stream The output stream (e.g., stdout, stderr).
 * @param prefix The prefix to include in the message (e.g., "Info", "Error").
 * @param format The format string (printf-style).
 * @param ... Additional arguments for the format string.
 * 
 * This function prints a formatted message prefixed with "[RGSL <prefix>]" to the given output stream.
 * It utilizes a dynamic buffer to accommodate messages of varying lengths.
 */
void rgsl_fprintf(FILE *stream, const char* prefix, const char* format, ...);

/**
 * @brief Prints an informational message to the standard output stream.
 * @param message The informational message to print.
 * 
 * This function prints an informational message prefixed with "[RGSL Info]" to stdout.
 * It utilizes the rgsl_fprint function for formatted output.
 * 
 * @code{c}
 * rgsl_print_info("Shader compiled successfully.\n");
 * @endcode
 */
void rgsl_print_info(const char* message);

/**
 * @brief Prints a formatted informational message to the standard output stream.
 * @param format The format string (printf-style).
 * @param ... Additional arguments for the format string.
 * 
 * This function prints a formatted informational message prefixed with "[RGSL Info]" to stdout.
 * It utilizes the rgsl_fprintf function for formatted output.
 * 
 * @code{c}
 * rgsl_printf_info("Shader %s compiled successfully in %d ms.\n", shader_name, time_ms);
 * @endcode
 */
void rgsl_printf_info(const char* format, ...);

/**
 * @brief Prints an error message to the standard error stream.
 * @param message The error message to print.
 * 
 * This function prints an error message prefixed with "[RGSL Error]" to stderr.
 * It utilizes the rgsl_fprint function for formatted output.
 * 
 * @code{c}
 * rgsl_print_error("An error occurred while processing the shader.\n");
 * @endcode
 */
void rgsl_print_error(const char* message);

/**
 * @brief Prints a formatted informational message to the standard output stream.
 * @param format The format string (printf-style).
 * @param ... Additional arguments for the format string.
 * 
 * This function prints a formatted informational message prefixed with "[RGSL Info]" to stdout.
 * It utilizes the rgsl_fprintf function for formatted output.
 * 
 * @code{c}
 * rgsl_printf_error("Failed to compile shader %s: %s\n", shader_name, error_message);
 * @endcode
 */
void rgsl_printf_error(const char* format, ...);
