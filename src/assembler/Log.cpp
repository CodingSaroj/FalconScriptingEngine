#include "Log.hpp"
#include "LogLevel.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Log::Log(LogLevel level, std::string log)
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

            std::cout<<Context::Line<<":"<<Context::Character<<": "<<log<<"\n";
        }
    }
}
