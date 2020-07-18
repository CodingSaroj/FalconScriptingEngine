#ifndef FALCON_ASM_SERIALIZE_HPP
#define FALCON_ASM_SERIALIZE_HPP

#include <iostream>

#include "Token.hpp"
#include "AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        void Serialize(Token token);
        void Serialize(ASTNode * node, std::string padding = "");
    }
}

#endif
