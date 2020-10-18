/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_VM_DISASSEMBLER_HPP
#define FALCON_VM_DISASSEMBLER_HPP

#include "common/OpCode.hpp"
#include "common/Register.hpp"

namespace Falcon 
{
    class Disassembler
    {
    public:
        static std::string DisassembleInstruction(uint8_t * code, uint64_t * targetIP = nullptr);

    private:
        static std::string DisassembleRegister(uint8_t reg);
    };
}

#endif
