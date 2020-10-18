/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_FALI_LITERAL_TRAITS_HPP
#define FALCON_FALI_LITERAL_TRAITS_HPP

#include "common/Common.hpp"

namespace Falcon
{
    namespace FALI
    {
        bool IsWholeLiteral(const std::string & varName);
        bool IsIntegerLiteral(const std::string & varName);
        bool IsRealLiteral(const std::string & varName);
    }
}

#endif
