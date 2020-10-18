#ifndef FALCON_FCC_FALCON_SCRIPT_TOKEN_HPP
#define FALCON_FCC_FALCON_SCRIPT_TOKEN_HPP

#include "common/Common.hpp"

namespace Falcon
{
    namespace FCC
    {
        namespace FalconScript
        {
            struct WordType
            {
                std::string Str;
            };

            struct OperatorType
            {
                std::string Type;
            };

            struct PunctuatorType
            {
                char Type;
            };

            struct Token
            {
                std::optional<bool>             Boolean;
                std::optional<char>             Character;
                std::optional<uint64_t>         Whole;
                std::optional<int64_t>          Integer;
                std::optional<double>           Real;
                std::optional<WordType>         Word;
                std::optional<OperatorType>     Operator;
                std::optional<PunctuatorType>   Punctuator;
                std::optional<std::string>      String;

                uint64_t LineNumber = 0;

                Token() = default;
                Token(uint64_t line, bool boolean);
                Token(uint64_t line, char character);
                Token(uint64_t line, uint64_t whole);
                Token(uint64_t line, int64_t integer);
                Token(uint64_t line, double real);
                Token(uint64_t line, WordType word);
                Token(uint64_t line, OperatorType op);
                Token(uint64_t line, PunctuatorType punctuator);
                Token(uint64_t line, const std::string & string);

                inline bool Valid()
                {
                    return Boolean || Character || Whole || Integer || Real || Word || Operator || Punctuator || String;
                }
            };
        }
    }
}

#endif
