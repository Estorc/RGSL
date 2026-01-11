/** ********************************************************************************
 * @section Packager_Overview Overview
 * @file packager.h
 * @brief Header file for shader packaging functions.
 * @details
 * Typical use cases:
 * - Packaging compiled shaders into C source files.
 * *********************************************************************************
 * @section Packager_Header Header
 * <RGSL/packager.h>
 ***********************************************************************************
 * @section Packager_Metadata Metadata
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
 * @brief Packages the given shaders into a C source file format.
 * @param shaders An array of shader_data structures containing shader codes to package.
 * @return true if packaging was successful, false otherwise.
 * 
 * This function takes an array of shader_data structures, each containing shader code
 * and its associated metadata, and writes them into a C source file specified in the
 * global RGSL options. The output file contains an array of shader blobs that can be
 * included in C/C++ projects.
 */
bool rgsl_package_shaders(struct rgsl_shader_data* shaders);