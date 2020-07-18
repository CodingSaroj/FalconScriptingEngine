#ifndef FALCON_ASM_LOG_HPP
#define FALCON_ASM_LOG_HPP

#include <iostream>

#include "LogLevel.hpp"
#include "Context.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Log
        {
            public:
                Log(LogLevel level, std::string log);
        };
    }
}

#endif
