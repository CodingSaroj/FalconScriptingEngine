#ifndef FALCON_FALI_FCC_FALCON_SCRIPT_LEXER_HPP
#define FALCON_FALI_FCC_FALCON_SCRIPT_LEXER_HPP

#include "common/Common.hpp"
#include "common/Logger.hpp"

#include "fali/AST.hpp"

#include "fcc/falconscript/Token.hpp"

namespace Falcon
{
    namespace FCC
    {
        namespace FalconScript
        {
            class Lexer
            {
            public:
                Lexer(const std::string & text);

                std::vector<Token> Lex();

            private:
                std::string m_Text;
            };
        }
    }
}

#endif
