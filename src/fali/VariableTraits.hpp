#ifndef FALCON_FALI_VARIABLE_TRAITS_HPP
#define FALCON_FALI_VARIABLE_TRAITS_HPP

#include "common/Common.hpp"

namespace Falcon
{
    namespace FALI
    {
        bool IsVariable(const std::string & str);
        bool IsLoaded(const std::string & str);

        uint8_t LoadVariableIntoRegister(const std::string & str);

        void ClearRegisters();
    }
}

#endif
