#ifndef FALCON_ASM_PARSER_HPP
#define FALCON_ASM_PARSER_HPP

#include <functional>

#include "Lexer.hpp"

#include "AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Parser
        {
            public:
                Parser(std::function<Token()> fetchToken, std::function<Token()> peek);

                ASTNode * parse();

            private:
                Token m_CurrentToken;

                std::function<Token()> m_FetchToken;
                std::function<Token()> m_Peek;

                constexpr void skipNewlines()
                {
                    while (m_CurrentToken.Type == TokenType::NEWLINE) { m_CurrentToken = m_FetchToken(); }
                }
                
                AtomNode        parseAtom();
                InstructionNode parseInstruction();
                LabelNode       parseLabel();
                RoutineNode     parseRoutine();
                CodeSectionNode parseCodeSection();

                DebugMetaNode     parseDebugMeta();
                DebugLineMapNode  parseDebugLineMap();
                DebugLocalVarNode parseDebugLocalVar();
                DebugRoutineNode  parseDebugRoutine();
                DebugSectionNode  parseDebugSection();

                ReflectionFunctionNode  parseReflectionFunction();
                ReflectionStructureNode parseReflectionStructure();
                ReflectionAliasNode     parseReflectionAlias();
                ReflectionSectionNode   parseReflectionSection();

                ASTNode * parseModule();
        };
    }
}

#endif
