/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef CHAKRA_CORE_STRING_FORMAT_HPP
#define CHAKRA_CORE_STRING_FORMAT_HPP

#include "common/Common.hpp"

namespace Falcon
{
    std::string FormatBase(std::string && format, std::initializer_list<std::string> args);
   
    template <typename Type>
    std::string FormatArg(const Type & t)
    {
        std::stringstream out;
        out<<t;
        return std::move(out.str());
    }
    
    template <typename... Args>
    std::string Format(std::string && format, Args... args)
    {
        return FormatBase(std::move(format), {std::move(FormatArg(args))...});
    }
}

#endif
