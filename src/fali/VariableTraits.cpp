/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "VariableTraits.hpp"

namespace Falcon
{
    namespace FALI
    {
        static std::array<std::string, 4> s_Registers;
        static uint8_t s_Cursor = 1;

        bool IsVariable(const std::string & str)
        {
            if ((str[0] >= 'a' && str[0] <= 'z') || (str[0] <= 'A' && str[0] >= 'Z') || str[0] == '_' || str[0] == '$')
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool IsLoaded(const std::string & str)
        {
            for (auto & reg : s_Registers)
            {
                if (str == reg)
                {
                    return true;
                }
            }

            return false;
        }

        uint8_t LoadVariableIntoRegister(const std::string & str)
        {
            for (size_t i = 0; i < 4; i++)
            {
                if (str == s_Registers[i])
                {
                    return i + 1;
                }
            }

            if (s_Cursor == 5)
            {
                s_Cursor = 1;
                s_Registers[s_Cursor] = str;
                return s_Cursor;
            }
            else
            {
                s_Registers[s_Cursor] = str;
                return s_Cursor++;
            }
        }

        void ClearRegisters()
        {
            for (auto & reg : s_Registers)
            {
                reg.clear();
            }

            s_Cursor = 1;
        }
    }
}
