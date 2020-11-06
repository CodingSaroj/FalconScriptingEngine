/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "Token.hpp"

namespace Falcon
{
    namespace FCC
    {
        namespace FalconScript
        {
            Token::Token(uint64_t line, bool boolean)
                : LineNumber(line), Boolean(boolean)
            {
            }

            Token::Token(uint64_t line, char character)
                : LineNumber(line), Character(character)
            {
            }

            Token::Token(uint64_t line, uint64_t whole)
                : LineNumber(line), Whole(whole)
            {
            }

            Token::Token(uint64_t line, int64_t integer)
                : LineNumber(line), Integer(integer)
            {
            }

            Token::Token(uint64_t line, double real)
                : LineNumber(line), Real(real)
            {
            }

            Token::Token(uint64_t line, const WordType & word)
                : LineNumber(line), Word(word)
            {
            }

            Token::Token(uint64_t line, const OperatorType & op)
                : LineNumber(line), Operator(op)
            {
            }

            Token::Token(uint64_t line, const PunctuatorType & punctuator)
                : LineNumber(line), Punctuator(punctuator)
            {
            }

            Token::Token(uint64_t line, const std::string & string)
                : LineNumber(line), String(string)
            {
            }
        }
    }
}
