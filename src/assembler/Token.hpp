#ifndef FALCON_ASM_TOKEN_HPP
#define FALCON_ASM_TOKEN_HPP

#include <iostream>

#include <cstdint>

#include "TokenTypes.hpp"

namespace Falcon
{
    namespace Assembler
    {
        struct Token
        {
            TokenType Type;

            union
            {
                char        Char;
                uint64_t    Uint;
                int64_t     Int;
                double      Float;
            };
            std::string Str;

            Token(TokenType type);
            Token(TokenType type, std::string value);
            Token(TokenType type, char value);
            Token(TokenType type, uint64_t value);
            Token(TokenType type, int64_t value);
            Token(TokenType type, double value);
        };
    }
}

#endif
