/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "Disassembler.hpp"

namespace Falcon
{
    std::string Disassembler::DisassembleRegister(uint8_t reg)
    {
        std::string regStr;

        if (reg & 0b00100000)
        {
            regStr = "[" + RegisterType::s_Names[reg & 0b00000011] + "]";
        }
        else if (reg & 0b00010000)
        {
            regStr = "@" + RegisterType::s_Names[reg & 0b00000011];
        }
        else
        {
            regStr = RegisterType::s_Names[reg];
        }

        return regStr;
    }

    std::string Disassembler::DisassembleInstruction(uint8_t * code, uint64_t * targetIP)
    {
        auto toLower = [](std::string str)->std::string
        {
            for (auto & c : str)
            {
                c = std::tolower(c);
            }
            return str;
        };

        uint64_t ip = 0;

        OpCode::OpCode op = (OpCode::OpCode)code[ip];
        OpCode::Layout layout = OpCode::s_Layouts[code[ip]];

        std::string disassembly(toLower(OpCode::s_Names[code[0]]));

        if (layout == OpCode::Layout::UN_REG)
        {
            disassembly += " " + toLower(DisassembleRegister(code[++ip]));
        }
        else if (layout == OpCode::Layout::UN_NUM)
        {
            if (op == OpCode::PSHNUL || op == OpCode::POPNUL || op == OpCode::RET)
            {
                uint32_t num = *(uint32_t *)&code[++ip];
                ip += 3;

                disassembly += " " + std::to_string(num);
            }
            else
            {
                uint64_t num = *(uint64_t *)&code[++ip];
                ip += 7;

                disassembly += " " + std::to_string(num);
            }
        }
        else if (layout == OpCode::Layout::UN_STR)
        {
            std::string str;

            char c = 0;

            ip++;

            while ((c = (char)code[ip]))
            {
                str += c;

                ip++;
            }

            disassembly = " " + str;
        }
        else if (layout == OpCode::Layout::BIN_REG_REG)
        {
            disassembly += " " + toLower(DisassembleRegister(code[ip + 1])) + " " + toLower(DisassembleRegister(code[ip + 2]));
            ip += 2;
        }
        else if (layout == OpCode::Layout::BIN_MOV)
        {
            uint8_t reg = code[++ip];

            disassembly += " " + toLower(DisassembleRegister(reg));

            if (op == OpCode::MOV8)
            {
                uint8_t num = code[++ip];

                disassembly += " " + std::to_string(+num);
            }
            else if (op == OpCode::MOV16)
            {
                uint16_t num = *(uint16_t *)&code[++ip];
                ip++;

                disassembly += " " + std::to_string(num);
            }
            else if (op == OpCode::MOV32 || (op >= OpCode::LOAD8 && op <= OpCode::GLODREF))
            {
                uint32_t num = *(uint32_t *)&code[++ip];
                ip += 3;

                disassembly += " " + std::to_string(num);
            }
            else if (op == OpCode::MOV64)
            {
                uint64_t num = *(uint64_t *)&code[++ip];
                ip += 7;

                disassembly += " " + std::to_string(num);
            }
        }
        else if (layout == OpCode::Layout::BIN_CALL)
        {
            uint32_t num = *(uint32_t *)&code[++ip];
            ip += 4;

            std::string str;

            char c = 0;

            while ((c = (char)code[ip]))
            {
                str += c;

                ip++;
            }

            disassembly += " " + std::to_string(num) + " " + str;
        }

        if (targetIP)
        {
            *targetIP += ip;
        }

        return disassembly;
    }
}
