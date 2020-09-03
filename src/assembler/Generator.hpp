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
            Generator(ASTNode * node, bool debug = false);

            std::string generate();

        private:
            ASTNode * m_Node;

            bool m_Debug;

            std::vector<std::pair<uint64_t, std::string>> m_UnresolvedSymbols;
            std::unordered_map<std::string, uint64_t>     m_LabelAddresses;
            std::unordered_map<std::string, uint64_t>     m_Symbols;

            std::string m_Bytecode;

            std::string generateSymbolTable();

            void generateInstruction(InstructionNode * inst);
            void generateRoutine(RoutineNode * routine);
            void generateCodeSection(CodeSectionNode * code);
            
            void generateDebugRoutine(DebugRoutineNode * routine);
            void generateDebugSection(DebugSectionNode * dbg);

            void generateReflectionFunction(ReflectionFunctionNode * function);
            void generateReflectionStructure(ReflectionStructureNode * structure);
            void generateReflectionAlias(ReflectionAliasNode * alias);
            void generateReflectionSection(ReflectionSectionNode * refl);

            void generateModule(ModuleNode * module);
        };
    }
}

#endif
