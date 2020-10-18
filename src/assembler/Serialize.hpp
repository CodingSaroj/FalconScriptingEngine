/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_ASM_SERIALIZE_HPP
#define FALCON_ASM_SERIALIZE_HPP

#include "common/Common.hpp"
#include "common/StringFormat.hpp"

#include "assembler/Token.hpp"
#include "assembler/AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        void Serialize(const Token & token);
        void Serialize(ASTNode * node, const std::string & padding = "");
    }
}

#endif
