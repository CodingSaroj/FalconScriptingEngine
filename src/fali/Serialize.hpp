#ifndef FALCON_FALI_SERIALIZE_HPP
#define FALCON_FALI_SERIALIZE_HPP

#include <iostream>

#include "fali/AST.hpp"
#include "fali/IR.hpp"

namespace Falcon
{
    namespace FALI
    {
        void Serialize(ASTNode * ast, const std::string & padding = "");
        void Serialize(IRNode * ir, const std::string & padding = "");
    }
}

#endif
