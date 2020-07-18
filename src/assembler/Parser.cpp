#include "Parser.hpp"
#include "AST.hpp"
#include "LogLevel.hpp"
#include "TokenTypes.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Parser::Parser(std::function<Token()> fetchToken, std::function<Token()> peek)
            : m_FetchToken(fetchToken), m_Peek(peek), m_CurrentToken(fetchToken())
        {
        }

        ASTNode * Parser::processAtom()
        {
            AtomNode * atom = nullptr;

            if (m_CurrentToken.Type == TokenType::CHAR)
            {
                atom = new AtomNode(m_CurrentToken.Char);
            }
            else if (m_CurrentToken.Type == TokenType::UINT)
            {
                atom = new AtomNode(m_CurrentToken.Uint);
            }
            else if (m_CurrentToken.Type == TokenType::INT)
            {
                atom = new AtomNode(m_CurrentToken.Int);
            }
            else if (m_CurrentToken.Type == TokenType::FLOAT)
            {
                atom = new AtomNode(m_CurrentToken.Float);
            }
            else if (m_CurrentToken.Type == TokenType::REGISTER)
            {
                atom = new AtomNode(m_CurrentToken.Str);
            }

            return (ASTNode *)atom;
        }

        ASTNode * Parser::processInstruction()
        {
            InstructionNode * inst = new InstructionNode(m_CurrentToken.Str);

            for (int i = 0; i < 2; i++)
            {
                TokenType type = m_Peek().Type;

                if (type == TokenType::REGISTER || type == TokenType::CHAR || type == TokenType::UINT || type == TokenType::INT || type == TokenType::FLOAT)
                {
                    m_CurrentToken = m_FetchToken();

                    inst->Args.emplace_back(*(AtomNode *)processAtom());
                }
                else
                {
                    break;
                }
            }

            return (ASTNode *)inst;
        }

        ASTNode * Parser::processRoutine()
        {
            std::string name = m_CurrentToken.Str;

            RoutineNode * routine = new RoutineNode(name);

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != (TokenType)':')
            {
                Log(LogLevel::ERR, "Expected a `:` after routine `" + name + "`.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::NEWLINE)
            {
                Log(LogLevel::ERR, "Expected new line after `:`.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            while (true)
            {
                if (m_CurrentToken.Type != TokenType::INSTRUCTION)
                {
                    while (m_CurrentToken.Type == TokenType::NEWLINE)
                    {
                        m_CurrentToken = m_FetchToken();
                    }

                    if (m_CurrentToken.Type != TokenType::INSTRUCTION)
                    {
                        break;
                    }
                }
                
                routine->Instructions.emplace_back(*(InstructionNode *)processInstruction());

                if ((m_CurrentToken = m_FetchToken()).Type != TokenType::NEWLINE)
                {
                    Log(LogLevel::ERR, "Expected new line after instruction in routine `" + name + "`.");
                    exit(2);
                }
                else
                {
                    m_CurrentToken = m_FetchToken();
                }
            }

            if (!routine->Instructions.size())
            {
                Log(LogLevel::WRN, "Empty routine `" + name + "`.");
            }

            return (ASTNode *)routine;
        }

        ASTNode * Parser::processSection()
        {
            if (m_CurrentToken.Type != TokenType::IDENTIFIER)
            {
                Log(LogLevel::ERR, "Expected section name after keyword `sect`.");
                exit(2);
            }

            auto validateSyntax =   [&]()
                                    {
                                        if ((m_CurrentToken = m_FetchToken()).Type != (TokenType)':')
                                        {
                                            Log(LogLevel::ERR, "Expected `:` after section name.");
                                            exit(2);
                                        }

                                        if ((m_CurrentToken = m_FetchToken()).Type != TokenType::NEWLINE)
                                        {
                                            Log(LogLevel::ERR, "Expected newline after `:`.");
                                            exit(2);
                                        }
                                    };

            if (m_CurrentToken.Str == "code")
            {
                validateSyntax();

                return processCodeSection();
            }
            else
            {
                Log(LogLevel::ERR, "Invalid section name `" + m_CurrentToken.Str + "`.");
                exit(2);
            }
        }

        ASTNode * Parser::processCodeSection()
        {
            CodeSectionNode * code = new CodeSectionNode();

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == (TokenType)'.')
            {
                m_CurrentToken = m_FetchToken();
                code->Routines.emplace_back(*(RoutineNode *)processRoutine());

                while (m_CurrentToken.Type == TokenType::NEWLINE)
                {
                    m_CurrentToken = m_FetchToken();
                }
            }

            return code;
        }

        ASTNode * Parser::parse()
        {
            ASTNode * node = nullptr;

            if (m_CurrentToken.Type == TokenType::SECTION)
            {
                m_CurrentToken = m_FetchToken();
                node = processSection();
            }

            return node;
        }
    }
}
