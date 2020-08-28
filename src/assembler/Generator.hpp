#ifndef FALCON_ASM_GENERATOR_HPP
#define FALCON_ASM_GENERATOR_HPP


#include <unordered_map>

#include <cstring>

#include "Parser.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Generator
        {
        public:
            Generator(ASTNode * node);

            std::string generate();

        private:
            ASTNode * m_Node;

            std::vector<std::pair<uint64_t, std::string>> m_UnresolvedSymbols;
            std::unordered_map<std::string, uint64_t>     m_LabelAddresses;
            std::unordered_map<std::string, uint64_t>     m_Symbols;

            std::string m_Bytecode;

            std::string generateSymbolTable();

            void generateInstruction(InstructionNode * inst);
            void generateRoutine(RoutineNode * routine);
            void generateCodeSection(CodeSectionNode * code);
        };
    }
}

#endif
