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
