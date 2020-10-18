#ifndef FALCON_FALI_SERIALIZE_HPP
#define FALCON_FALI_SERIALIZE_HPP

#include "common/Common.hpp"
#include "common/Logger.hpp"

#include "fali/AST.hpp"
//#include "fali/IR.hpp"

namespace Falcon
{
    namespace FALI
    {
        void Serialize(ASTNode * ast, std::string padding = "");
        //void Serialize(IRFunction & function);
    }
}

#endif
