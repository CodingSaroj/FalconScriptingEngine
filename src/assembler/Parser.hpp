/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_ASM_PARSER_HPP
#define FALCON_ASM_PARSER_HPP

#include "common/Common.hpp"
#include "common/Logger.hpp"

#include "assembler/Lexer.hpp"
#include "assembler/AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Parser
        {
        public:
            Parser(std::function<Token()> fetchToken, std::function<Token()> peek);

            ASTNode * Parse();

        private:
            Token m_CurrentToken;

            std::function<Token()> m_FetchToken;
            std::function<Token()> m_Peek;

            constexpr void SkipNewlines()
            {
                while (m_CurrentToken.NewLine) { m_CurrentToken = m_FetchToken(); }
            }
            
            AtomNode        ParseAtom();
            InstructionNode ParseInstruction();
            LabelNode       ParseLabel();
            RoutineNode     ParseRoutine();
            CodeSectionNode ParseCodeSection();

            DebugMetaNode     ParseDebugMeta();
            DebugLineMapNode  ParseDebugLineMap();
            DebugLocalVarNode ParseDebugLocalVar();
            DebugRoutineNode  ParseDebugRoutine();
            DebugSectionNode  ParseDebugSection();

            ReflectionAttributeNode ParseReflectionAttribute();
            ReflectionFunctionNode  ParseReflectionFunction();
            ReflectionStructureNode ParseReflectionStructure();
            ReflectionAliasNode     ParseReflectionAlias();
            ReflectionSectionNode   ParseReflectionSection();

            ASTNode * ParseModule();
        };
    }
}

#endif
