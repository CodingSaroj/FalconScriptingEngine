/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_DEBUGGER_DATA_HPP
#define FALCON_DEBUGGER_DATA_HPP

#include "common/Common.hpp"

namespace Falcon
{
    struct DebugData
    {
        struct DebuggerFunction
        {
            std::string Signature;
            uint64_t StartLine;
            uint64_t EndLine;
            std::unordered_map<std::string, std::pair<std::string, uint64_t>> LocalVariables;
        };

        std::map<uint64_t, std::pair<uint64_t, std::string>> LineData;
        std::unordered_map<std::string, DebuggerFunction>    FunctionData;
    };
}

#endif
