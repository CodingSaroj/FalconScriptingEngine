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
                    std::cout<<(Common::Colors::White | Common::Colors::Bold)<<"Info";
                    break;

                case LogLevel::WRN:
                    std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"Warning";
                    break;

                case LogLevel::ERR:
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Error";
                    break;
            }

            std::cout<<Common::Colors::White<<":"<<line<<":"<<character<<": "<<log<<"\n";
        }
    }
}
