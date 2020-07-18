#ifndef FALCON_ASM_CONTEXT_HPP
#define FALCON_ASM_CONTEXT_HPP

#include <cstdint>

namespace Falcon
{
    namespace Assembler
    {
        struct Context
        {
            static uint64_t Line;
            static uint64_t Character;
        };
    }
}

#endif
