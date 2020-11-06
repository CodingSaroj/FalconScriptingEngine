/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "CLIColors.hpp"

#ifdef _WIN32
    #include <Windows.h>
#endif

namespace Falcon
{
    namespace Common
    {
        namespace Colors
        {
            std::ostream & operator<<(std::ostream & out, const Format & fmt)
            {
                #if defined(_WIN32)
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

                    // Win32 Console doesn't support bold/italic

                    if (fmt.test(2))
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    }
                    else if (fmt.test(3))
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
                    }
                    else if (fmt.test(4))
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
                    }
                    else if (fmt.test(5))
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
                    }
                    else if (fmt.test(6))
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                    }
                #elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
                    out.write("\033[0;", 4);

                    if (fmt.test(0))
                    {
                        out.write("1;", 2);
                    }
                    else if (fmt.test(1))
                    {
                        out.write("3;", 2);
                    }

                    if (fmt.test(2))
                    {
                        out.write("0", 1);
                    }
                    else if (fmt.test(3))
                    {
                        out.write("31", 2);
                    }
                    else if (fmt.test(4))
                    {
                        out.write("34", 2);
                    }
                    else if (fmt.test(5))
                    {
                        out.write("32", 2);
                    }
                    else if (fmt.test(6))
                    {
                        out.write("33", 2);
                    }

                    out.write("m", 1);
                #endif

                return out;
            }
        }
    }
}
