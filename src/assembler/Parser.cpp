#include "Parser.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Parser::Parser(std::function<Token()> fetchToken, std::function<Token()> peek)
            : m_FetchToken(fetchToken), m_Peek(peek), m_CurrentToken(fetchToken())
        {
        }

        AtomNode Parser::processAtom()
        {
            AtomNode atom('\0');

            if (m_CurrentToken.Type == TokenType::CHAR) { atom = AtomNode(m_CurrentToken.Char); }

            else if (m_CurrentToken.Type == TokenType::UINT) { atom = AtomNode(m_CurrentToken.Uint); }

            else if (m_CurrentToken.Type == TokenType::INT) { atom = AtomNode(m_CurrentToken.Int); }

            else if (m_CurrentToken.Type == TokenType::FLOAT) { atom = AtomNode(m_CurrentToken.Float); }

            else if (m_CurrentToken.Type == TokenType::REGISTER) { atom = AtomNode(m_CurrentToken.Str); }

            else if (m_CurrentToken.Type == TokenType::IDENTIFIER) { atom = AtomNode(m_CurrentToken.Str); }

            atom.Line = Context::Line;
            atom.Character = Context::Character;

            return atom;
        }

        InstructionNode Parser::processInstruction()
        {
            InstructionNode inst(m_CurrentToken.Str);

            inst.Line = Context::Line;
            inst.Character = Context::Character;

            for (int i = 0; i < 2; i++)
            {
                TokenType type = m_Peek().Type;

                if (type == TokenType::IDENTIFIER || type == TokenType::REGISTER || type == TokenType::CHAR || type == TokenType::UINT || type == TokenType::INT || type == TokenType::FLOAT)
                {
                    m_CurrentToken = m_FetchToken();

                    inst.Args.emplace_back(processAtom());
                }
                else
                {
                    break;
                }
            }

            return inst;
        }

        LabelNode Parser::processLabel()
        {
            std::string name = m_CurrentToken.Str;

            LabelNode label(name);

            label.Line = Context::Line;
            label.Character = Context::Character;

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
                
                label.Instructions.emplace_back(processInstruction());

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

            if (!label.Instructions.size()) { Log(LogLevel::WRN, "Empty label `" + name + "`."); }

            return label;
        }

        RoutineNode Parser::processRoutine()
        {
            std::string & name = m_CurrentToken.Str;

            RoutineNode routine(name);

            routine.Line = Context::Line;
            routine.Character = Context::Character;
            
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
                routine.Labels.emplace_back(processLabel());

                while (m_CurrentToken.Type == TokenType::NEWLINE) { m_CurrentToken = m_FetchToken(); }
            }

            if (!routine.Labels.size()) { Log(LogLevel::WRN, "Empty routine `" + name + "`."); }

            return routine;
        }        

        CodeSectionNode Parser::processCodeSection()
        {
            CodeSectionNode code;

            code.Line = Context::Line;
            code.Character = Context::Character;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                code.Routines.emplace_back(processRoutine());

                while (m_CurrentToken.Type == TokenType::NEWLINE) { m_CurrentToken = m_FetchToken(); }
            }

            return code;
        }

        DebugMetaNode Parser::processDebugMeta()
        {
            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                return DebugMetaNode(m_CurrentToken.Str);
            }
            else
            {
                Log(LogLevel::ERR, "Expected identifier|str as 1st argument to `meta`.\n");
                exit(2);
            }
        }

        DebugLineMapNode Parser::processDebugLineMap()
        {
            uint64_t startLoc, lineNum;
            std::string lineData;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::UINT)
            {
                startLoc = m_CurrentToken.Uint;
            }
            else
            {
                Log(LogLevel::ERR, "Expected uint as 1st argument to `map`.\n");
                exit(2);
            }
            
            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::UINT)
            {
                lineNum = m_CurrentToken.Uint;
            }
            else
            {
                Log(LogLevel::ERR, "Expected uint as 2nd argument to `map`.\n");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                lineData = m_CurrentToken.Str;
            }
            else
            {
                Log(LogLevel::ERR, "Expected identifier|str as 3rd argument to `map`.\n");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            return DebugLineMapNode(startLoc, lineNum, lineData);
        }

        DebugLocalVarNode Parser::processDebugLocalVar()
        {
            std::string name, type;
            uint64_t stackOffset;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                name = m_CurrentToken.Str;
            }
            else
            {
                Log(LogLevel::ERR, "Expected identifier|str as 1st argument to `local`.\n");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                type = m_CurrentToken.Str;
            }
            else
            {
                Log(LogLevel::ERR, "Expected identifier|str as 2nd argument to `local`.\n");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::UINT)
            {
                stackOffset = m_CurrentToken.Uint;
            }
            else
            {
                Log(LogLevel::ERR, "Expected uint as 3rd argument to `local`.\n");
                exit(2);
            }

            return DebugLocalVarNode(name, type, stackOffset);
        }
        
        DebugRoutineNode Parser::processDebugRoutine()
        {
            std::string & name = m_CurrentToken.Str;

            DebugRoutineNode routine(name, "");

            routine.Line = Context::Line;
            routine.Character = Context::Character;
            
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

            if (m_CurrentToken.Type == TokenType::META)
            {
                routine.MetaData = processDebugMeta();
            }

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::NEWLINE) { m_CurrentToken = m_FetchToken(); }

            while (m_CurrentToken.Type == TokenType::MAP || m_CurrentToken.Type == TokenType::LOCAL)
            {
                auto statementType = m_CurrentToken.Type;

                if (statementType == TokenType::MAP)
                {
                    routine.LineMaps.emplace_back(processDebugLineMap());
                }
                else
                {
                    routine.LocalVariables.emplace_back(processDebugLocalVar());
                }

                m_CurrentToken = m_FetchToken();

                while (m_CurrentToken.Type == TokenType::NEWLINE) { m_CurrentToken = m_FetchToken(); }
            }

            return routine;

        }

        DebugSectionNode Parser::processDebugSection()
        {
            DebugSectionNode dbg;

            dbg.Line = Context::Line;
            dbg.Character = Context::Character;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                dbg.Routines.emplace_back(processDebugRoutine());

                while (m_CurrentToken.Type == TokenType::NEWLINE) { m_CurrentToken = m_FetchToken(); }
            }

            return dbg;
        }
        
        ASTNode * Parser::processModule()
        {
            ModuleNode * module = new ModuleNode();

            if (m_CurrentToken.Type != TokenType::SECTION)
            {
                Log(LogLevel::ERR, "Expected `sect` at toplevel.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::IDENTIFIER)
            {
                Log(LogLevel::ERR, "Expected module name after keyword `sect`.");
                exit(2);
            }

            auto validateSyntax =   [&]()
                                    {
                                        if ((m_CurrentToken = m_FetchToken()).Type != (TokenType)':')
                                        {
                                            Log(LogLevel::ERR, "Expected `:` after module name.");
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

                module->CodeSection = processCodeSection();
            }
            else
            {
                Log(LogLevel::ERR, "Invalid module name `" + m_CurrentToken.Str + "`.");
                exit(2);
            }

            if (m_CurrentToken.Type != TokenType::SECTION)
            {
                Log(LogLevel::ERR, "Expected `sect` at toplevel.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::IDENTIFIER)
            {
                Log(LogLevel::ERR, "Expected module name after keyword `sect`.");
                exit(2);
            }

            if (m_CurrentToken.Str == "debug")
            {
                validateSyntax();

                module->DebugSection = processDebugSection();
            }
            else
            {
                Log(LogLevel::ERR, "Invalid module name `" + m_CurrentToken.Str + "`.");
                exit(2);
            }

            return module;
        }

        ASTNode * Parser::parse()
        {
            return processModule();
        }
    }
}
