#include "Log.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Log::Log(LogLevel level, std::string log, uint64_t line, uint64_t character)
        {
            switch(level)
            {
                case LogLevel::INF:
                    std::cout<<"Info:";
                    break;

                case LogLevel::WRN:
                    std::cout<<"Warning:";
                    break;

                case LogLevel::ERR:
                    std::cout<<"Error:";
                    break;
            }

            std::cout<<line<<":"<<character<<": "<<log<<"\n";
        }
    }
}
