/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_ASM_SEMANTIC_ANALYZER_HPP
#define FALCON_ASM_SEMANTIC_ANALYZER_HPP

#include "common/Common.hpp"

#include "assembler/Parser.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class SemanticAnalyzer
        {
        public:
            explicit SemanticAnalyzer(ASTNode * node);

            ASTNode * Analyze();

        private:
            ASTNode * m_Node;

            CodeSectionNode * m_Code;
            RoutineNode *     m_Routine;
            
            bool FindInCodeSection(const std::string & name);
            bool FindInRoutine(const std::string & name);

            void AnalyzeInstruction(InstructionNode * inst);
            void AnalyzeRoutine();
            void AnalyzeCodeSection();
        };
    }
}

#endif
