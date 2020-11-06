/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "DebugContext.hpp"

namespace Falcon
{
    namespace FALI
    {
        DebugContext::DebugContext(uint8_t * code, DebugData debugData, Debugger::DebuggerFunctions debuggerFunctions, const std::string & debuggerName, Debugger::PrintVarFunction printVarFunction)
            : m_Debugger(code, debugData, debuggerFunctions, debuggerName, printVarFunction)
        {
            FILE ** stdfiles = (FILE **)m_Debugger.GetGlobalData(0);
            stdfiles[0] = stdin;
            stdfiles[1] = stdout;
            stdfiles[2] = stderr;

            AddExternalFunction("malloc", std::function<uint64_t(uint64_t)>(
                [&](uint64_t size)->uint64_t
                {
                    return (uint64_t)malloc(size);
                }
            ));

            AddExternalFunction("free", std::function<void(uint64_t)>(
                [&](uint64_t Address)
                {
                    free((void *)Address);
                }
            ));

            AddExternalFunction("realloc", std::function<uint64_t(uint64_t, uint64_t)>(
                [&](uint64_t Address, uint64_t size)->uint64_t
                {
                    return (uint64_t)realloc((void *)Address, size);
                }
            ));

            AddExternalFunction("fopen", std::function<uint64_t(uint64_t, uint64_t)>(
                [&](uint64_t filename, uint64_t openmode)->uint64_t
                {
                    return (uint64_t)fopen((const char *)filename, (const char *)openmode);
                }
            ));

            AddExternalFunction("fclose", std::function<void(uint64_t)>(
                [&](uint64_t file)
                {
                    fclose((FILE *)file);
                }
            ));

            AddExternalFunction("fwrite", std::function<uint64_t(uint64_t, uint64_t, uint64_t, uint64_t)>(
                [&](uint64_t data, uint64_t size, uint64_t count, uint64_t file)->uint64_t
                {
                    return fwrite((void *)data, size, count, (FILE *)file);
                }
            ));

            AddExternalFunction("fread", std::function<uint64_t(uint64_t, uint64_t, uint64_t, uint64_t)>(
                [&](uint64_t data, uint64_t size, uint64_t count, uint64_t file)->uint64_t
                {
                    return fread((void *)data, size, count, (FILE *)file);
                }
            ));

            AddExternalFunction("memcpy", std::function<uint64_t(uint64_t, uint64_t, uint64_t)>(
                [&](uint64_t dest, uint64_t src, uint64_t size)->uint64_t
                {
                    return (uint64_t)memcpy((void *)dest, (void *)src, size);
                }
            ));

            AddExternalFunction("memmove", std::function<uint64_t(uint64_t, uint64_t, uint64_t)>(
                [&](uint64_t dest, uint64_t src, uint64_t size)->uint64_t
                {
                    return (uint64_t)memmove((void *)dest, (void *)src, size);
                }
            ));
            
            AddExternalFunction("memset", std::function<uint64_t(uint64_t, uint8_t, uint64_t)>(
                [&](uint64_t dest, uint8_t byte, uint64_t size)->uint64_t
                {
                    return (uint64_t)memset((void *)dest, byte, size);
                }
            ));

            AddExternalFunction("raise", std::function<uint32_t(uint8_t)>(
                [&](uint8_t sig)->uint32_t
                {
                    return raise(sig);
                }
            ));

            AddExternalFunction("exit", std::function<void(uint8_t)>(
                [&](uint8_t status)
                {
                    exit(status);
                }
            ));

            for (auto & function : Function::GetIterable())
            {
                if (!function.second.Base)
                {
                    function.second.Base = new Function(Function::FunctionType(
                        [&](std::vector<void *> args)->void *
                        {
                            FLCN_ASSERT(args.size() != function.second.ParameterTypes.size(), "FALI",
                                "Function '{}' requires {} arguments, {} provided.", function.first, function.second.ParameterTypes.size(), args.size()
                            );
                    
                            uint64_t argsSize = 0;
                            
                            for (int i = 0; i < function.second.ParameterTypes.size(); i++)
                            {
                                argsSize += Object::GetObjectData(function.second.ParameterTypes[i]).Size;

                                m_Debugger.Push((uint8_t *)args[i], Object::GetObjectData(function.second.ParameterTypes[i]).Size);
                            }

                            m_Debugger.Run(function.second.Name, argsSize);

                            return (void *)m_Debugger.Pop(Object::GetObjectData(function.second.ReturnType).Size);
                        }
                    ));
                }
            }
        }

        void DebugContext::Shell()
        {
            m_Debugger.Shell();
        }
    }
}
