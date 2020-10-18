/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "LiteralTraits.hpp"

namespace Falcon
{
    namespace FALI
    {
        bool IsWholeLiteral(const std::string & varName)
        {
            bool isWhole = true;

            for (char c : varName)
            {
                if (c < '0' && c > '9')
                {
                    isWhole = false;
                    break;
                }
            }

            return isWhole;
        }

        bool IsIntegerLiteral(const std::string & varName)
        {
            if (varName[0] == '+' || varName[0] == '-')
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool IsRealLiteral(const std::string & varName)
        {
            if (varName.find('.') == std::string::npos)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
