#include "Logger.hpp"

namespace Falcon
{
    std::ostream * Logger::s_Out = &std::cout;
    
    void Logger::SetOStream(std::ostream & out)
    {
        s_Out = &out;
    }

    void Logger::LogInfo(const std::string & section, const std::string & log)
    {
        std::ostream & out = *s_Out;

        out<<Format("{}Info{}::{}: {}\n", Common::Colors::Blue | Common::Colors::Bold, Common::Colors::White, section, log);
    }

    void Logger::LogWarning(const std::string & section, const std::string & log)
    {
        std::ostream & out = *s_Out;

        out<<Format("{}Warning{}::{}: {}\n", Common::Colors::Yellow | Common::Colors::Bold, Common::Colors::White, section, log);
    }

    void Logger::LogError(const std::string & section, const std::string & log)
    {
        std::ostream & out = *s_Out;

        out<<Format("{}Error{}::{}: {}\n", Common::Colors::Red | Common::Colors::Bold, Common::Colors::White, section, log);
    }

    void Logger::LogFatalError(const std::string & section, const std::string & log)
    {
        std::cerr<<Format("{}Error{}::{}: {}\n", Common::Colors::Red | Common::Colors::Bold, Common::Colors::White, section, log);
    }
}
