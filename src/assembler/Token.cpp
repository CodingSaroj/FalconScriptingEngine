/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "../../pch/FalconPCH.hpp"

#include "Token.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Token::Token(uint64_t line, SectionType sect)
            : LineNumber(line), Section(sect)
        {
        }

        Token::Token(uint64_t line, const InstructionType & inst)
            : LineNumber(line), Instruction(inst)
        {
        }

        Token::Token(uint64_t line, const RegisterType & reg)
            : LineNumber(line), Register(reg)
        {
        }
        
        Token::Token(uint64_t line, char value)
            : LineNumber(line), Char(value)
        {
        }

        Token::Token(uint64_t line, uint64_t value)
            : LineNumber(line), Uint(value)
        {
        }

        Token::Token(uint64_t line, int64_t value)
            : LineNumber(line), Int(value)
        {
        }

        Token::Token(uint64_t line, double value)
            : LineNumber(line), Float(value)
        {
        }

        Token::Token(uint64_t line, const std::string & value)
            : LineNumber(line), Str(value)
        {
        }

        Token::Token(uint64_t line, ArbitaroryType arbitarory)
            : LineNumber(line), Arbitarory(arbitarory)
        {
        }

        Token::Token(uint64_t line, NewLineType newline)
            : LineNumber(line), NewLine(newline)
        {
        }
    }
}
