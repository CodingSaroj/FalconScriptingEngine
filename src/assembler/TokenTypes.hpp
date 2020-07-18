#ifndef FALCON_ASM_TOKEN_TYPES_HPP
#define FALCON_ASM_TOKEN_TYPES_HPP

#include <cstdint>

namespace Falcon
{
    namespace Assembler
    {
        enum class TokenType : uint8_t
        {
            SECTION = 128,
            IDENTIFIER,
            INSTRUCTION,
            REGISTER,
            CHAR,
            UINT,
            INT,
            FLOAT,
            NEWLINE
        };
    }
}

#endif
