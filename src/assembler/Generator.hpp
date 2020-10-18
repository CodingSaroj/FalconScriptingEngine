#ifndef FALCON_ASM_GENERATOR_HPP
#define FALCON_ASM_GENERATOR_HPP

#include "common/Common.hpp"
#include "common/Endian.hpp"

#include "assembler/Parser.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Generator
        {
        public:
            Generator(ASTNode * node, bool debug = false);

            std::string Generate();

        private:
            ASTNode * m_Node;

            bool m_Debug;

            std::vector<std::pair<uint64_t, std::string>> m_UnresolvedSymbols;
            std::unordered_map<std::string, uint64_t>     m_LabelAddresses;
            std::unordered_map<std::string, uint64_t>     m_Symbols;

            std::string m_Bytecode;

            std::string m_CodeSection;
            std::string m_DebugSection;
            std::string m_ReflectionSection;

            std::string GenerateSymbolTable();

            void GenerateInstruction(InstructionNode * inst);
            void GenerateRoutine(RoutineNode * routine);
            void GenerateCodeSection(CodeSectionNode * code);
            
            void GenerateDebugRoutine(DebugRoutineNode * routine);
            void GenerateDebugSection(DebugSectionNode * dbg);

            void GenerateReflectionAttribute(ReflectionAttributeNode * attrib);
            void GenerateReflectionFunction(ReflectionFunctionNode * function);
            void GenerateReflectionStructure(ReflectionStructureNode * structure);
            void GenerateReflectionAlias(ReflectionAliasNode * alias);
            void GenerateReflectionSection(ReflectionSectionNode * refl);

            void GenerateModule(ModuleNode * module);
        };
    }
}

#endif
