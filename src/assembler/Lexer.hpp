/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_LEXER_HPP
#define FALCON_LEXER_HPP

#include "common/Common.hpp"
#include "common/Logger.hpp"
#include "common/OpCode.hpp"
#include "common/Register.hpp"

#include "assembler/Token.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Lexer
        {
        public:
            explicit Lexer(const std::string & text);

            Token Lex();
            Token Peek();

        private:
            std::vector<Token> m_Tokens;
            size_t m_Cursor;
        };
    }
}

#endif
