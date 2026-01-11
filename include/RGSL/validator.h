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
#include <RGSL/rgsl.h>
#include <stdbool.h>

/**
 * @brief Structure to map shader languages to their validator functions.
 * 
 * This structure holds the name of a shader language and a pointer to the function
 * that validates shaders written in that language.
 */
struct rgsl_language_validator_mapping {
    const char* language;
    bool (*validator_func)(struct rgsl_shader_data *);
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
bool rgsl_validate_shader(struct rgsl_shader_data * shader);