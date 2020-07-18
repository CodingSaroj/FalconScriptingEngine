#include "Token.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Token::Token(TokenType type)
            : Type(type)
        {
        }

        Token::Token(TokenType type, std::string value)
            : Type(type), Str(value)
        {
        }
        
        Token::Token(TokenType type, char value)
            : Type(type), Char(value)
        {
        }

        Token::Token(TokenType type, uint64_t value)
            : Type(type), Uint(value)
        {
        }

        Token::Token(TokenType type, int64_t value)
            : Type(type), Int(value)
        {
        }

        Token::Token(TokenType type, double value)
            : Type(type), Float(value)
        {
        }
    }
}
