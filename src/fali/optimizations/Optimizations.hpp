#ifndef FALCON_FALI_OPTIMIZATIONS_HPP
#define FALCON_FALI_OPTIMIZATIONS_HPP

#include <algorithm>

#include "fali/IR.hpp"

namespace Falcon
{
    namespace FALI
    {
        namespace Optimizations
        {
            IRModuleNode * MultipleContinuousAssignment(IRModuleNode * module);
            IRModuleNode * UnusedStatementResult(IRModuleNode * module);
            IRModuleNode * UnusedVariable(IRModuleNode * module);
            IRModuleNode * ConstantFolding(IRModuleNode * module);
        }
    }
}

#endif
