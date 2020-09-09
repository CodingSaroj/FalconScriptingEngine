#include "Parser.hpp"

namespace Falcon
{
    namespace Assembler
    {
        static void processEscapeSequences(std::string & str)
        {
            size_t backSlashLoc = str.find('\\');

            while (backSlashLoc != std::string::npos)
            {
                std::string num;

                switch (str[backSlashLoc + 1])
                {
                    case 'a':
                    {
                        str.erase(backSlashLoc + 1, 1);
                        str.replace(backSlashLoc, 1, "\a");
                        break;
                    }

                    case 'b':
                    {
                        str.erase(backSlashLoc + 1, 1);
                        str.replace(backSlashLoc, 1, "\b");
                        break;
                    }

                    case 'd':
                    {
                        str.erase(backSlashLoc + 1, 1);
                        str.replace(backSlashLoc, 1, "\"");
                        break;
                    }

                    case 'n':
                    {
                        str.erase(backSlashLoc + 1, 1);
                        str.replace(backSlashLoc, 1, "\n");
                        break;
                    }

                    case 'q':
                    {
                        str.erase(backSlashLoc + 1, 1);
                        str.replace(backSlashLoc, 1, "\'");
                        break;
                    }

                    case 'r':
                    {
                        str.erase(backSlashLoc + 1, 1);
                        str.replace(backSlashLoc, 1, "\r");
                        break;
                    }

                    case 't':
                    {
                        str.erase(backSlashLoc + 1, 1);
                        str.replace(backSlashLoc, 1, "\t");
                        break;
                    }

                    case 'x':
                    {
                        for (uint8_t i = 2; i < 4; i++)
                        {
                            if ((str[backSlashLoc + i] >= '0' && str[backSlashLoc + i] <= '9')
                                || (str[backSlashLoc + i] >= 'a' && str[backSlashLoc + i] <= 'f')
                                || (str[backSlashLoc + i] >= 'A' && str[backSlashLoc + i] <= 'F'))
                            {
                                num += str[backSlashLoc + i];
                            }
                            else
                            {
                                Log(LogLevel::ERR, "Expected 0-9|a-f|A-F after `\\x` in string.");
                                exit(2);
                            }
                        }

                        char data = (char)strtoul(num.c_str(), nullptr, 16);

                        str.erase(backSlashLoc + 1, 3);
                        str.replace(backSlashLoc, 1, &data);
                        break;
                    }

                    case '\\':
                    {
                        str.erase(backSlashLoc + 1, 1);
                        str.replace(backSlashLoc, 1, "\\");
                        backSlashLoc++;
                        break;
                    }

                    case '0':
                    {
                        num += '0';
                        
                        for (uint8_t i = 2; i < 4; i++)
                        {
                            if ((str[backSlashLoc + i] >= '0' && str[backSlashLoc + i] <= '7'))
                            {
                                num += str[backSlashLoc + i];
                            }
                            else
                            {
                                break;
                            }
                        }

                        char data = (char)strtoul(num.c_str(), nullptr, 8);

                        str.erase(backSlashLoc + 1, 4);
                        str.replace(backSlashLoc, 1, &data);
                        break;
                    }

                    default:
                    {
                        Log(LogLevel::ERR, "Invalid escape sequence `\\" + std::string(&str[backSlashLoc], 1) + "`.");
                        exit(2);
                    }
                }

                backSlashLoc = str.find('\\', backSlashLoc);
            }
        }

        Parser::Parser(std::function<Token()> fetchToken, std::function<Token()> peek)
            : m_FetchToken(fetchToken), m_Peek(peek), m_CurrentToken(fetchToken())
        {
        }

        AtomNode Parser::parseAtom()
        {
            AtomNode atom('\0');

            if (m_CurrentToken.Type == TokenType::CHAR) { atom = AtomNode(m_CurrentToken.Char); }

            else if (m_CurrentToken.Type == TokenType::UINT) { atom = AtomNode(m_CurrentToken.Uint); }

            else if (m_CurrentToken.Type == TokenType::INT) { atom = AtomNode(m_CurrentToken.Int); }

            else if (m_CurrentToken.Type == TokenType::FLOAT) { atom = AtomNode(m_CurrentToken.Float); }

            else if (m_CurrentToken.Type == TokenType::REGISTER) { atom = AtomNode(m_CurrentToken.Str); }

            else if (m_CurrentToken.Type == TokenType::IDENTIFIER) { atom = AtomNode(m_CurrentToken.Str); processEscapeSequences(atom.Str); }

            atom.Line = Context::Line;
            atom.Character = Context::Character;

            return atom;
        }

        InstructionNode Parser::parseInstruction()
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

                    inst.Args.emplace_back(parseAtom());
                }
                else
                {
                    break;
                }
            }

            return inst;
        }

        LabelNode Parser::parseLabel()
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
                
                label.Instructions.emplace_back(parseInstruction());

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

        RoutineNode Parser::parseRoutine()
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
                routine.Labels.emplace_back(parseLabel());

                skipNewlines();
            }

            if (!routine.Labels.size()) { Log(LogLevel::WRN, "Empty routine `" + name + "`."); }

            return routine;
        }        

        CodeSectionNode Parser::parseCodeSection()
        {
            CodeSectionNode code;

            code.Line = Context::Line;
            code.Character = Context::Character;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                code.Routines.emplace_back(parseRoutine());

                skipNewlines();
            }

            return code;
        }

        DebugMetaNode Parser::parseDebugMeta()
        {
            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::IDENTIFIER)
            {
                Log(LogLevel::ERR, "Expected identifier|str as 1st argument to `meta`.");
                exit(2);
            }

            std::string signature(m_CurrentToken.Str);

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::UINT)
            {
                Log(LogLevel::ERR, "Expected uint as 2nd argument to `meta`.");
                exit(2);
            }

            uint64_t startLine = m_CurrentToken.Uint;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::UINT)
            {
                Log(LogLevel::ERR, "Expected uint as 3rd argument to `meta`.");
                exit(2);
            }

            uint64_t endLine = m_CurrentToken.Uint;

            return DebugMetaNode(signature, startLine, endLine);
        }

        DebugLineMapNode Parser::parseDebugLineMap()
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
                Log(LogLevel::ERR, "Expected uint as 1st argument to `map`.");
                exit(2);
            }
            
            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::UINT)
            {
                lineNum = m_CurrentToken.Uint;
            }
            else
            {
                Log(LogLevel::ERR, "Expected uint as 2nd argument to `map`.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                lineData = m_CurrentToken.Str;
            }
            else
            {
                Log(LogLevel::ERR, "Expected identifier|str as 3rd argument to `map`.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            return DebugLineMapNode(startLoc, lineNum, lineData);
        }

        DebugLocalVarNode Parser::parseDebugLocalVar()
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
        
        DebugRoutineNode Parser::parseDebugRoutine()
        {
            std::string & name = m_CurrentToken.Str;

            DebugRoutineNode routine(name, "", 0, 0);

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

            if (m_CurrentToken.Type == TokenType::IDENTIFIER && m_CurrentToken.Str == "META")
            {
                routine.MetaData = parseDebugMeta();
            }

            m_CurrentToken = m_FetchToken();

            skipNewlines();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER && (m_CurrentToken.Str == "MAP" || m_CurrentToken.Str == "LOCAL"))
            {
                auto statementType = m_CurrentToken.Type;

                if (m_CurrentToken.Str == "MAP")
                {
                    routine.LineMaps.emplace_back(parseDebugLineMap());
                }
                else
                {
                    routine.LocalVariables.emplace_back(parseDebugLocalVar());
                }

                m_CurrentToken = m_FetchToken();

                skipNewlines();
            }

            return routine;

        }

        DebugSectionNode Parser::parseDebugSection()
        {
            DebugSectionNode dbg;

            dbg.Line = Context::Line;
            dbg.Character = Context::Character;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                dbg.Routines.emplace_back(parseDebugRoutine());

                skipNewlines();
            }

            return dbg;
        }
        
        ReflectionAttributeNode Parser::parseReflectionAttribute()
        {
            std::string name;
            std::vector<std::string> attributes;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                name = m_CurrentToken.Str;
            }
            else
            {
                Log(LogLevel::ERR, "Expected identifier after `attribute` in reflection section.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();
            
            if (m_CurrentToken.Type != TokenType::IDENTIFIER || m_CurrentToken.Str != "[")
            {
                Log(LogLevel::ERR, "Expected `[` after target name in reflection section.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                attributes.emplace_back(m_CurrentToken.Str);

                m_CurrentToken = m_FetchToken();

                skipNewlines();

                if (m_CurrentToken.Type != (TokenType)',' && (m_CurrentToken.Type != TokenType::IDENTIFIER || m_CurrentToken.Str != "]"))
                {
                    Log(LogLevel::ERR, "Expected `,` or `]` after attribute.");
                    exit(2);
                }
                
                if (m_CurrentToken.Type == TokenType::IDENTIFIER && m_CurrentToken.Str == "]")
                {
                    break;
                }

                m_CurrentToken = m_FetchToken();

                skipNewlines();
            }
            
            if (m_CurrentToken.Type != TokenType::IDENTIFIER || m_CurrentToken.Str != "]")
            {
                Log(LogLevel::ERR, "Expected `]` after attributes in reflection section.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            return ReflectionAttributeNode(name, attributes);
        }

        ReflectionFunctionNode Parser::parseReflectionFunction()
        {
            std::string name, retType;
            std::vector<std::string> params;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                retType = m_CurrentToken.Str;
            }
            else
            {
                Log(LogLevel::ERR, "Expected return type after `function` in reflection section.");
                exit(2);
            }                   

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                name = m_CurrentToken.Str;
            }
            else
            {
                Log(LogLevel::ERR, "Expected function name after return type in reflection section.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != (TokenType)'(')
            {
                Log(LogLevel::ERR, "Expected `(` after function name in reflection section.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                params.emplace_back(m_CurrentToken.Str);

                m_CurrentToken = m_FetchToken();

                skipNewlines();

                if (m_CurrentToken.Type != (TokenType)',' && m_CurrentToken.Type != (TokenType)')')
                {
                    Log(LogLevel::ERR, "Expected `,` or `)` after parameter type.");
                    exit(2);
                }
                
                if (m_CurrentToken.Type == (TokenType)')')
                {
                    break;
                }

                m_CurrentToken = m_FetchToken();

                skipNewlines();
            }
            
            if (m_CurrentToken.Type != (TokenType)')')
            {
                Log(LogLevel::ERR, "Expected `)` after parameters in reflection section.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            return ReflectionFunctionNode(name, retType, params);
        }

        ReflectionStructureNode Parser::parseReflectionStructure()
        {
            std::string name;
            std::vector<std::pair<std::string, std::string>> members;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                name = m_CurrentToken.Str;
            }
            else
            {
                Log(LogLevel::ERR, "Expected struct name after `struct` in reflection section.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != (TokenType)'{')
            {
                Log(LogLevel::ERR, "Expected `{` after struct name in reflection section.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER)
            {
                members.emplace_back(m_CurrentToken.Str, "");

                m_CurrentToken = m_FetchToken();

                if (m_CurrentToken.Type == TokenType::IDENTIFIER)
                {
                    members[members.size() - 1].second = m_CurrentToken.Str;
                }

                m_CurrentToken = m_FetchToken();

                skipNewlines();

                if (m_CurrentToken.Type != (TokenType)',' && m_CurrentToken.Type != (TokenType)'}')
                {
                    Log(LogLevel::ERR, "Expected `,` or `}` after member name in reflection section.");
                    exit(2);
                }

                if (m_CurrentToken.Type == (TokenType)'}')
                {
                    m_CurrentToken = m_FetchToken();
                    break;
                }

                m_CurrentToken = m_FetchToken();

                skipNewlines();
            }
            
            return ReflectionStructureNode(name, members);
        }

        ReflectionAliasNode Parser::parseReflectionAlias()
        {
            std::string name, base;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::IDENTIFIER)
            {
                Log(LogLevel::ERR, "Expected alias name after `alias` in reflection section.");
                exit(2);
            }

            name = m_CurrentToken.Str;

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::IDENTIFIER)
            {
                Log(LogLevel::ERR, "Expected base type name after alias name in reflection section.");
                exit(2);
            }

            base = m_CurrentToken.Str;

            return ReflectionAliasNode(name, base);
        }

        ReflectionSectionNode Parser::parseReflectionSection()
        {
            ReflectionSectionNode refl;

            refl.Line = Context::Line;
            refl.Character = Context::Character;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Type == TokenType::IDENTIFIER && (m_CurrentToken.Str == "ATTRIBUTE" || m_CurrentToken.Str == "FUNCTION" || m_CurrentToken.Str == "STRUCT" || m_CurrentToken.Str == "ALIAS"))
            {
                if (m_CurrentToken.Str == "ATTRIBUTE")
                {
                    refl.Attributes.emplace_back(std::move(parseReflectionAttribute()));
                }
                else if (m_CurrentToken.Str == "FUNCTION")
                {
                    refl.Functions.emplace_back(std::move(parseReflectionFunction())); 
                }
                else if(m_CurrentToken.Str == "STRUCT")
                {
                    refl.Structures.emplace_back(std::move(parseReflectionStructure()));
                }
                else if (m_CurrentToken.Str == "ALIAS")
                {
                    refl.Aliases.emplace_back(std::move(parseReflectionAlias()));
                }
                
                skipNewlines();
            }

            return refl;
        }
        
        ASTNode * Parser::parseModule()
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

            if (m_CurrentToken.Str != "code")
            {
                Log(LogLevel::ERR, "Invalid section name `" + m_CurrentToken.Str + "`.");
                exit(2);
            }
            
            validateSyntax();

            module->CodeSection = std::move(parseCodeSection());

            skipNewlines();

            if (m_CurrentToken.Type != TokenType::SECTION)
            {
                Log(LogLevel::ERR, "Expected `sect` at toplevel.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::IDENTIFIER)
            {
                Log(LogLevel::ERR, "Expected section name after keyword `sect`.");
                exit(2);
            }

            if (m_CurrentToken.Str != "debug")
            {
                Log(LogLevel::ERR, "Invalid section name `" + m_CurrentToken.Str + "`.");
                exit(2);
            }

            validateSyntax();

            module->DebugSection = std::move(parseDebugSection());

            skipNewlines();

            if (m_CurrentToken.Type != TokenType::SECTION)
            {
                Log(LogLevel::ERR, "Expected `sect` at toplevel.");
                exit(2);
            }

            m_CurrentToken = m_FetchToken();

            if (m_CurrentToken.Type != TokenType::IDENTIFIER)
            {
                Log(LogLevel::ERR, "Expected section name after keyword `sect`.");
                exit(2);
            }

            if (m_CurrentToken.Str != "reflection")
            {
                Log(LogLevel::ERR, "Invalid section name `" + m_CurrentToken.Str + "`.");
                exit(2);
            }

            validateSyntax();

            module->ReflectionSection = std::move(parseReflectionSection());

            return module;
        }

        ASTNode * Parser::parse()
        {
            return parseModule();
        }
    }
}
