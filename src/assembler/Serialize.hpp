#ifndef FALCON_ASM_SERIALIZE_HPP
#define FALCON_ASM_SERIALIZE_HPP

#include <iostream>

#include "assembler/Token.hpp"
#include "assembler/AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        void Serialize(Token token);
        void Serialize(ASTNode * node, std::string padding = "");
    }
}

#endif
