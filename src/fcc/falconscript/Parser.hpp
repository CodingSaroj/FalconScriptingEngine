#ifndef FALCON_FCC_FALCONSCRIPT_PARSER_HPP
#define FALCON_FCC_FALCONSCRIPT_PARSER_HPP

#include "common/Common.hpp"
#include "common/Logger.hpp"

#include "fali/AST.hpp"
#include "fali/Types.hpp"

#include "fcc/falconscript/Token.hpp"

namespace Falcon
{
    namespace FCC
    {
        namespace FalconScript
        {
            class Parser
            {
            public:
                explicit Parser(const std::vector<Token> & tokens);
                explicit Parser(std::vector<Token> && tokens);

                FALI::ModuleNode Parse();

            private:
                std::vector<Token> m_Tokens;
                size_t m_Cursor;

                FALI::ModuleNode *      m_CurrentModule;
                FALI::ClassNode *       m_CurrentClass;
                FALI::FunctionNode *    m_CurrentFunction;
                std::string             m_CurrentNamespace;

                inline Token & Current()
                {
                    return m_Tokens[m_Cursor - 1];
                }

                inline Token & Advance()
                {
                    Token & token = m_Tokens[m_Cursor];

                    if (token.Valid())
                    {
                        m_Cursor++;
                    }

                    return token;
                }

                void AddValue(FALI::ValueNode & root, const FALI::ValueNode & value);
                void AddUnaryOperator(FALI::ValueNode & root, char opType);
                void AddBinaryOperator(FALI::ValueNode & root, char opType);

                bool                IsType();

                FALI::TypeFlag      ParseTypeFlags();
                FALI::DataType      ParseType();

                FALI::IfNode        ParseIfNode();
                FALI::ElseNode      ParseElseNode();
                FALI::WhileNode     ParseWhileNode();
                FALI::OperatorNode  ParseExpression(char terminator = ';');
                FALI::StatementNode ParseStatement(bool lastStatementWasIf);
                FALI::FunctionNode  ParseFunction(bool member = false, const std::string & className = "", bool specialMember = false, const std::string & specialMemberName = "");
                FALI::ClassNode     ParseClass();
                void                ParseNamespace();
                FALI::ModuleNode    ParseModule();
            };
        }
    }
}

#endif
