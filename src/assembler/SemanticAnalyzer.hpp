#ifndef FALCON_ASM_SEMANTIC_ANALYZER_HPP
#define FALCON_ASM_SEMANTIC_ANALYZER_HPP

#include <array>

#include "assembler/Parser.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class SemanticAnalyzer
        {
            public:
                SemanticAnalyzer(ASTNode * node);

                ASTNode * analyze();

            private:
                ASTNode * m_Node;

                CodeSectionNode * m_Code;
                RoutineNode *     m_Routine;
                
                bool findInCodeSection(const std::string & name);
                bool findInRoutine(const std::string & name);

                void analyzeInstruction(InstructionNode * inst);
                void analyzeRoutine();
                void analyzeCodeSection();
        };
    }
}

#endif
