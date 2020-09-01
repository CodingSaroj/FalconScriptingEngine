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
                
                AtomNode          processAtom();
                InstructionNode   processInstruction();
                LabelNode         processLabel();
                RoutineNode       processRoutine();
                CodeSectionNode   processCodeSection();

                DebugMetaNode     processDebugMeta();
                DebugLineMapNode  processDebugLineMap();
                DebugLocalVarNode processDebugLocalVar();
                DebugRoutineNode  processDebugRoutine();
                DebugSectionNode  processDebugSection();

                ASTNode * processModule();
        };
    }
}

#endif
