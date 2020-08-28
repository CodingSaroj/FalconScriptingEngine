#include "Parser.hpp"

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

            if (m_CurrentToken.Type == TokenType::CHAR) { atom = new AtomNode(m_CurrentToken.Char); }

            else if (m_CurrentToken.Type == TokenType::UINT) { atom = new AtomNode(m_CurrentToken.Uint); }

            else if (m_CurrentToken.Type == TokenType::INT) { atom = new AtomNode(m_CurrentToken.Int); }

            else if (m_CurrentToken.Type == TokenType::FLOAT) { atom = new AtomNode(m_CurrentToken.Float); }

            else if (m_CurrentToken.Type == TokenType::REGISTER) { atom = new AtomNode(m_CurrentToken.Str); }

            else if (m_CurrentToken.Type == TokenType::IDENTIFIER) { atom = new AtomNode(m_CurrentToken.Str); }

            atom->Line = Context::Line;
            atom->Character = Context::Character;

            return (ASTNode *)atom;
        }

        ASTNode * Parser::processInstruction()
        {
            InstructionNode * inst = new InstructionNode(m_CurrentToken.Str);

            inst->Line = Context::Line;
            inst->Character = Context::Character;

            for (int i = 0; i < 2; i++)
            {
                TokenType type = m_Peek().Type;

                if (type == TokenType::IDENTIFIER || type == TokenType::REGISTER || type == TokenType::CHAR || type == TokenType::UINT || type == TokenType::INT || type == TokenType::FLOAT)
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

        ASTNode * Parser::processLabel()
        {
            std::string name = m_CurrentToken.Str;

            LabelNode * label = new LabelNode(name);

            label->Line = Context::Line;
            label->Character = Context::Character;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != (TokenType)':')
            {
                Log(LogLevel::ERR, "Expected a `:` after label `" + name + "`.");
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
                
                label->Instructions.emplace_back(*(InstructionNode *)processInstruction());

                if ((m_CurrentToken = m_FetchToken()).Type != TokenType::NEWLINE)
                {
                    Log(LogLevel::ERR, "Expected new line after instruction in label `" + name + "`.");
                    exit(2);
                }
                else
                {
                    m_CurrentToken = m_FetchToken();
                }
            }

            if (!label->Instructions.size()) { Log(LogLevel::WRN, "Empty label `" + name + "`."); }

            return (ASTNode *)label;
        }

        ASTNode * Parser::processRoutine()
        {
            std::string & name = m_CurrentToken.Str;

            RoutineNode * routine = new RoutineNode(name);

            routine->Line = Context::Line;
            routine->Character = Context::Character;
            
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

            while (m_CurrentToken.Type == (TokenType)'.')
            {
                m_CurrentToken = m_FetchToken();
                routine->Labels.emplace_back(*(LabelNode *)processLabel());

                while (m_CurrentToken.Type == TokenType::NEWLINE) { m_CurrentToken = m_FetchToken(); }
            }

            if (!routine->Labels.size()) { Log(LogLevel::WRN, "Empty routine `" + name + "`."); }

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

            code->Line = Context::Line;
            code->Character = Context::Character;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                code->Routines.emplace_back(*(RoutineNode *)processRoutine());

                while (m_CurrentToken.Type == TokenType::NEWLINE) { m_CurrentToken = m_FetchToken(); }
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
