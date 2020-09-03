#ifndef FALCON_ASM_TOKEN_TYPES_HPP
#define FALCON_ASM_TOKEN_TYPES_HPP

#include <cstdint>

namespace Falcon
{
    namespace Assembler
    {
        enum class TokenType : uint8_t
        {
            // Top-level tokens
            SECTION = 128,
            IDENTIFIER,

            // Code section tokens
            INSTRUCTION,
            REGISTER,

            // General Tokens
            CHAR,
            UINT,
            INT,
            FLOAT,
            NEWLINE
        };
    }
}

#endif
