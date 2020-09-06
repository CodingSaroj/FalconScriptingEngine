#include <iostream>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include "Context.hpp"

namespace Falcon
{
    namespace FALI
    {
        Context::Context(uint8_t * code)
            : m_VM(code)
        {
            addExternalFunction("initGlobalMemory",
                (std::function<void(uint64_t)>) [&](uint64_t size)
                                                {
                                                    m_GlobalMemory.Size = size + 24;

                                                    m_GlobalMemory.Data = new uint8_t[m_GlobalMemory.Size];

                                                    auto data = (uint64_t *)m_GlobalMemory.Data;
                                                    data[0] = (uint64_t)stdin;
                                                    data[1] = (uint64_t)stdout;
                                                    data[2] = (uint64_t)stderr;
                                                });

            addExternalFunction("getGlobalAddress",
                (std::function<uint64_t(uint64_t)>) [&](uint64_t offset)->uint64_t
                                                    {
                                                        if (offset >= m_GlobalMemory.Size)
                                                        {
                                                            return 0;
                                                        }

                                                        return (uint64_t)(m_GlobalMemory.Data + offset);
                                                    });

            addExternalFunction("malloc",
                (std::function<uint64_t(uint64_t)>) [&](uint64_t size)->uint64_t
                                                    {
                                                        return (uint64_t)malloc(size);
                                                    });

            addExternalFunction("free",
                (std::function<void(uint64_t)>) [&](uint64_t address)
                                                {
                                                    free((void *)address);
                                                });

            addExternalFunction("realloc",
                (std::function<uint64_t(uint64_t, uint64_t)>)   [&](uint64_t address, uint64_t size)->uint64_t
                                                                {
                                                                    return (uint64_t)realloc((void *)address, size);
                                                                });

            addExternalFunction("fopen",
                (std::function<uint64_t(uint64_t, uint64_t)>)   [&](uint64_t filename, uint64_t openmode)->uint64_t
                                                                {
                                                                    return (uint64_t)fopen((const char *)filename, (const char *)openmode);
                                                                });

            addExternalFunction("fclose",
                (std::function<void(uint64_t)>) [&](uint64_t file)
                                                {
                                                    fclose((FILE *)file);
                                                });

            addExternalFunction("fwrite",
                (std::function<uint64_t(uint64_t, uint64_t, uint64_t, uint64_t)>)   [&](uint64_t data, uint64_t size, uint64_t count, uint64_t file)->uint64_t
                                                                                    {
                                                                                        return fwrite((void *)data, size, count, (FILE *)file);
                                                                                    });
            addExternalFunction("fread",
                (std::function<uint64_t(uint64_t, uint64_t, uint64_t, uint64_t)>)   [&](uint64_t data, uint64_t size, uint64_t count, uint64_t file)->uint64_t
                                                                                    {
                                                                                        return fread((void *)data, size, count, (FILE *)file);
                                                                                    });

            addExternalFunction("memcpy",
                (std::function<uint64_t(uint64_t, uint64_t, uint64_t)>) [&](uint64_t dest, uint64_t src, uint64_t size)->uint64_t
                                                                        {
                                                                            return (uint64_t)memcpy((void *)dest, (void *)src, size);
                                                                        });

            addExternalFunction("memmove",
                (std::function<uint64_t(uint64_t, uint64_t, uint64_t)>) [&](uint64_t dest, uint64_t src, uint64_t size)->uint64_t
                                                                        {
                                                                            return (uint64_t)memmove((void *)dest, (void *)src, size);
                                                                        });
            
            addExternalFunction("memset",
                (std::function<uint64_t(uint64_t, uint8_t, uint64_t)>) [&](uint64_t dest, uint8_t byte, uint64_t size)->uint64_t
                                                                        {
                                                                            return (uint64_t)memset((void *)dest, byte, size);
                                                                        });

            addExternalFunction("raise",
                (std::function<uint32_t(uint8_t)>)  [&](uint8_t sig)->uint32_t
                                                    {
                                                        return raise(sig);
                                                    });

            addExternalFunction("exit",
                (std::function<void(uint8_t)>)  [&](uint8_t status)
                                                {
                                                    exit(status);
                                                });

            for (auto & function : Function::GetIterable())
            {
                if (!function.second.Base)
                {
                    function.second.Base = new Function((Function::FunctionType)[&](std::vector<void *> args)->void *
                    {
                        if (args.size() != function.second.ParameterTypes.size())
                        {
                            std::cout<<"Function `"<<function.first<<"` requires "<<function.second.ParameterTypes.size()<<" arguments, "<<args.size()<<" provided.\n";
                            exit(2);
                        }
                    
                        uint64_t argsSize = 0;
                        
                        for (int i = 0; i < function.second.ParameterTypes.size(); i++)
                        {
                            argsSize += Object::GetObjectData(function.second.ParameterTypes[i]).Size;

                            m_VM.push((uint8_t *)args[i], Object::GetObjectData(function.second.ParameterTypes[i]).Size);
                        }

                        m_VM.run(function.second.Name, argsSize);

                        return (void *)m_VM.pop(Object::GetObjectData(function.second.ReturnType).Size);
                    });
                }
            }
        }
    }
}
