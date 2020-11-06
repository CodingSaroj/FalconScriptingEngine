/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "Mangle.hpp"

namespace Falcon
{
    namespace FALI
    {
        std::string MangleFunction(const std::string & name, std::vector<std::string> params)
        {
            std::string mangledName(MangleIdentifier(name));

            for (auto param : params)
            {
                mangledName += '#';
                mangledName += param;
            }

            mangledName += '#';

            return mangledName;
        }

        std::string DemangleFunction(const std::string & name)
        {
            std::string demangledName(name);

            size_t location = demangledName.find('#');

            if (location != std::string::npos)
            {
                demangledName.erase(location);
            }

            return demangledName;
        }

        std::string MangleIdentifier(const std::string & name)
        {
            std::string mangledName;

            for (size_t i = 0; i < name.size(); i++)
            {
                if (name[i] == '.')
                {
                    mangledName += '#';
                }
                else if (name[i] == '<')
                {
                    mangledName += '$';
                }
                else if (name[i] == '>')
                {
                    mangledName += '!';
                }
                else if (name[i] == ',')
                {
                    mangledName += '%';
                }
                else if (name[i] != ' ')
                {
                    mangledName += name[i];
                }
            }

            return mangledName;
        }

        std::string DemangleIdentifier(const std::string & name)
        {
            std::string demangledName;

            for (size_t i = 0; i < name.size(); i++)
            {
                if (name[i] == '#')
                {
                    demangledName += '.';
                }
                else if (name[i] == '$')
                {
                    demangledName += '<';
                }
                else if (name[i] == '!')
                {
                    demangledName += '>';
                }
                else if (name[i] == '%')
                {
                    demangledName += ',';
                }
                else
                {
                    demangledName += name[i];
                }
            }

            return demangledName;
        }
    }
}
