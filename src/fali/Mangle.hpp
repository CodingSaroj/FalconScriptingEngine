/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_FALI_MANGLE_HPP
#define FALCON_FALI_MANGLE_HPP

#include "common/Common.hpp"

namespace Falcon
{
    namespace FALI
    {
        std::string MangleFunction(const std::string & name, std::vector<std::string> params);
        std::string DemangleFunction(const std::string & name);

        std::string MangleIdentifier(const std::string & name);
        std::string DemangleIdentifier(const std::string & name);
    }
}

#endif
