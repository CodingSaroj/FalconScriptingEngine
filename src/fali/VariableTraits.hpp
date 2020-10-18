/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_FALI_VARIABLE_TRAITS_HPP
#define FALCON_FALI_VARIABLE_TRAITS_HPP

#include "common/Common.hpp"

namespace Falcon
{
    namespace FALI
    {
        bool IsVariable(const std::string & str);
        bool IsLoaded(const std::string & str);

        uint8_t LoadVariableIntoRegister(const std::string & str);

        void ClearRegisters();
    }
}

#endif
