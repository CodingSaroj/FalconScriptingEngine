/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "../../pch/FalconPCH.hpp"

#include "Types.hpp"

namespace Falcon
{
    namespace FALI
    {
        std::string DataType::Str() const
        {
            std::string str;

            if (Flags & TypeFlags::CONST)
            {
                str += "const ";
            }
            
            if (Flags & TypeFlags::TEMPARORY)
            {
                str += "tmp ";
            }

            str += Name;

            if (Flags & TypeFlags::REFERENCE)
            {
                str += " ref";
            }
            
            return std::move(str);
        }

        std::ostream & operator<<(std::ostream & out, const DataType & type)
        {
            out<<type.Str();

            return out;
        }
    }
}
