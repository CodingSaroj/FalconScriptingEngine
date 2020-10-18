/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
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
