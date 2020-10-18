#ifndef FALCON_ASM_TOKEN_HPP
#define FALCON_ASM_TOKEN_HPP

#include "common/Common.hpp"

namespace Falcon
{
    namespace Assembler
    {
        struct Token
        {
            struct SectionType{};

            struct InstructionType
            {
                std::string Name;
            };

            struct RegisterType
            {
                std::string Name;
            };

            struct ArbitaroryType
            {
                char Type;
            };

            struct NewLineType{};

            std::optional<SectionType>      Section;
            std::optional<InstructionType>  Instruction;
            std::optional<RegisterType>     Register;
            std::optional<char>             Char;
            std::optional<uint64_t>         Uint;
            std::optional<int64_t>          Int;
            std::optional<double>           Float;
            std::optional<std::string>      Str;
            std::optional<ArbitaroryType>   Arbitarory;
            std::optional<NewLineType>      NewLine;

            uint64_t LineNumber = 0;

            Token() = default;
            Token(uint64_t line, SectionType sect);
            Token(uint64_t line, const InstructionType & inst);
            Token(uint64_t line, const RegisterType & reg);
            Token(uint64_t line, char value);
            Token(uint64_t line, uint64_t value);
            Token(uint64_t line, int64_t value);
            Token(uint64_t line, double value);
            Token(uint64_t line, const std::string & value);
            Token(uint64_t line, ArbitaroryType value);
            Token(uint64_t line, NewLineType newline);

            constexpr bool Valid() { return Section || Instruction || Register || Char || Uint || Int || Float || Str || Arbitarory || NewLine; }
        };
    }
}

#endif
