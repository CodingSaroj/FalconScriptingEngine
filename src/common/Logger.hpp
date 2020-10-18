#ifndef CHAKRA_CORE_LOGGER_HPP
#define CHAKRA_CORE_LOGGER_HPP

#include "common/Common.hpp"
#include "common/CLIColors.hpp"
#include "common/StringFormat.hpp"

namespace Falcon
{
    class Logger
    {
    public:
        static void SetOStream(std::ostream & out);

        static void LogInfo(const std::string & section, const std::string & log);
        static void LogWarning(const std::string & section, const std::string & log);
        static void LogError(const std::string & section, const std::string & log);
        static void LogFatalError(const std::string & section, const std::string & log);

    private:
        static std::ostream * s_Out;
    };

    #ifdef DEBUG
        #if FLCN_LOG_LEVEL >= 1
            #define FLCN_INFO(section, ...) Logger::LogInfo(section, Format(__VA_ARGS__))
        #endif

        #if FLCN_LOG_LEVEL >= 2
            #define FLCN_WARNING(section, ...) Logger::LogWarning(section, Format(__VA_ARGS__))
        #endif

        #if FLCN_LOG_LEVEL == 3
            #define FLCN_ERROR(section, ...) Logger::LogError(section, Format(__VA_ARGS__))

            #define FLCN_ASSERT(cond, section, ...) if (!(cond)) { Logger::LogFatalError(section, Format(__VA_ARGS__)); exit(2); }
        #endif
    #else
        #define FLCN_INFO(...)
        #define FLCN_WARNING(...)
        #define FLCN_ERROR(...)

        #define FLCN_ASSERT(cond, ...)
    #endif

    #define FLCN_REL_INFO(section, ...) Logger::LogInfo(section, Format(__VA_ARGS__))
    #define FLCN_REL_WARNING(section, ...) Logger::LogWarning(section, Format(__VA_ARGS__))
    #define FLCN_REL_ERROR(section, ...) Logger::LogError(section, Format(__VA_ARGS__))
    #define FLCN_REL_ASSERT(cond, section, ...) if (!(cond)) { Logger::LogFatalError(section, Format(__VA_ARGS__)); exit(2); }
}

#endif
