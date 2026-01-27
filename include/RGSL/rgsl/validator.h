/** ********************************************************************************
 * @section RGSL_Validator_Overview Overview
 * @file validator.h
 * @brief Header file for RGSL shader validation functions.
 * @details
 * Typical use cases:
 * - Validating RGSL shader code snippets.
 * *********************************************************************************
 * @section RGSL_Validator_Header Header
 * <RGSL/rgsl/validator.h>
 ***********************************************************************************
 * @section RGSL_Validator_Metadata Metadata
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
#include <RGSL/validator.h>

/**
 * @brief Validates the given RGSL shader code.
 * @param shader The RGSL shader data to validate.
 * @return true if the shader code is valid, false otherwise.
 * 
 * This function performs validation specific to RGSL shader code.
 */
bool rgsl_rgsl_validate_shader(struct rgsl_shader_data * shader);
