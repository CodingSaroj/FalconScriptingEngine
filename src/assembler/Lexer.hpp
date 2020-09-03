#ifndef FALCON_LEXER_HPP
#define FALCON_LEXER_HPP

#include <algorithm>
#include <iostream>
#include <vector> 

#include "../vm/OpCode.hpp"
#include "../vm/Register.hpp"

#include "Context.hpp"
#include "Log.hpp"

#include "Token.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Lexer
        {
            public:
                Lexer(const std::string & text);

                Token lex();
                Token peek();

            private:
                const std::string m_Text;

                uint64_t m_Cursor;
                char     m_CurrentChar;

                Token lexNumber(bool signedInt = false);
                Token lexChar();
                Token lexStr();

                inline char advance()
                {
                    if (m_Cursor + 1 < m_Text.size())
                    {
                        Context::Character++;
                        return m_CurrentChar = m_Text[++m_Cursor];
                    }
                    else
                    {
                        return m_CurrentChar = '\0';
                    }
                }
        };
    }
}

#endif
