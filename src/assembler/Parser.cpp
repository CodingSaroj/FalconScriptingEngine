/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "Parser.hpp"

namespace Falcon
{
    namespace Assembler
    {
        static void ProcessEscapeSequences(std::string & str, uint64_t line)
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
                                FLCN_REL_ASSERT(false, "Assembler::Parser", "{}: Expected 0-9|a-f|A-F after `\\x` in string.", line);
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
                        FLCN_REL_ASSERT(false, "Assembler::Parser", "{}: Invalid escape sequence `\\{}`.", line, std::string(&str[backSlashLoc], 1));
                    }
                }

                backSlashLoc = str.find('\\', backSlashLoc);
            }
        }

        Parser::Parser(std::function<Token()> fetchToken, std::function<Token()> peek)
            : m_FetchToken(fetchToken), m_Peek(peek), m_CurrentToken(fetchToken())
        {
        }

        AtomNode Parser::ParseAtom()
        {
            AtomNode atom('\0');

            atom.Line = m_CurrentToken.LineNumber;

            if (m_CurrentToken.Char) { atom = AtomNode(*m_CurrentToken.Char); }

            else if (m_CurrentToken.Uint) { atom = AtomNode(*m_CurrentToken.Uint); }

            else if (m_CurrentToken.Int) { atom = AtomNode(*m_CurrentToken.Int); }

            else if (m_CurrentToken.Float) { atom = AtomNode(*m_CurrentToken.Float); }

            else if (m_CurrentToken.Register) { atom = AtomNode(m_CurrentToken.Register->Name); }

            else if (m_CurrentToken.Str) { atom = AtomNode(*m_CurrentToken.Str); ProcessEscapeSequences(*atom.Str, m_CurrentToken.LineNumber); }

            return std::move(atom);
        }

        InstructionNode Parser::ParseInstruction()
        {
            InstructionNode inst(m_CurrentToken.Instruction->Name);

            inst.Line = m_CurrentToken.LineNumber;

            for (int i = 0; i < 2; i++)
            {
                Token token = m_Peek();

                if (token.Str || token.Register || token.Char || token.Uint || token.Int || token.Float)
                {
                    m_CurrentToken = m_FetchToken();

                    inst.Args.emplace_back(std::move(ParseAtom()));
                }
                else
                {
                    break;
                }
            }

            return std::move(inst);
        }

        LabelNode Parser::ParseLabel()
        {
            std::string name = *m_CurrentToken.Str;

            LabelNode label(name);

            label.Line = m_CurrentToken.LineNumber;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Arbitarory && m_CurrentToken.Arbitarory->Type == ':', "Assembler::Parser", "{}: Expected a `:` after label `{}`.", m_CurrentToken.LineNumber, name);

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.NewLine, "Assembler::Parser", "{}: Expected newline after `:`.", m_CurrentToken.LineNumber);

            SkipNewlines();

            while (true)
            {
                FLCN_REL_ASSERT(m_CurrentToken.Instruction, "Assembler::Parser", "{}: Expected instruction in label `{}`.", m_CurrentToken.LineNumber, name);

                label.Instructions.emplace_back(std::move(ParseInstruction()));
                
                FLCN_REL_ASSERT((m_CurrentToken = m_FetchToken()).NewLine, "Assembler::Parser", "{}: Expected newline after instruction in label `{}`.", m_CurrentToken.LineNumber, name);

                SkipNewlines();

                if (!m_CurrentToken.Instruction)
                {
                    break;
                }
            }

            if (label.Instructions.empty())
            {
                FLCN_REL_WARNING("Assembler::Parser", "{}: Empty label `{}`.", m_CurrentToken.LineNumber, name);
            }

            return std::move(label);
        }

        RoutineNode Parser::ParseRoutine()
        {
            std::string & name = *m_CurrentToken.Str;

            RoutineNode routine(name);

            routine.Line = m_CurrentToken.LineNumber;
            
            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Arbitarory && m_CurrentToken.Arbitarory->Type == ':', "Assembler::Parser", "{}: Expected a `:` after routine `{}`.", m_CurrentToken.LineNumber, name);

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.NewLine, "Assembler::Parser", "{}: Expected newline after `:`.", m_CurrentToken.LineNumber);

            SkipNewlines();

            while (m_CurrentToken.Arbitarory && m_CurrentToken.Arbitarory->Type == '.')
            {
                m_CurrentToken = m_FetchToken();
                routine.Labels.emplace_back(std::move(ParseLabel()));

                SkipNewlines();
            }

            if (routine.Labels.empty())
            {
                FLCN_REL_WARNING("Assembler::Parser", "{}: Empty routine `{}`.", m_CurrentToken.LineNumber, name);
            }

            return std::move(routine);
        }

        CodeSectionNode Parser::ParseCodeSection()
        {
            CodeSectionNode code;

            code.Line = m_CurrentToken.LineNumber;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Str)
            {
                SkipNewlines();

                code.Routines.emplace_back(std::move(ParseRoutine()));

                SkipNewlines();
            }

            return std::move(code);
        }

        DebugMetaNode Parser::ParseDebugMeta()
        {
            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as 1st argument to `meta`.", m_CurrentToken.LineNumber);

            std::string signature(*m_CurrentToken.Str);

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Uint, "Assembler::Parser", "{}: Expected uint as 2st argument to `meta`.", m_CurrentToken.LineNumber);

            uint64_t startLine = *m_CurrentToken.Uint;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Uint, "Assembler::Parser", "{}: Expected uint as 3rd argument to `meta`.", m_CurrentToken.LineNumber);

            uint64_t endLine = *m_CurrentToken.Uint;

            DebugMetaNode meta(signature, startLine, endLine);

            meta.Line = m_CurrentToken.LineNumber;

            return std::move(meta);
        }

        DebugLineMapNode Parser::ParseDebugLineMap()
        {
            uint64_t startLoc, lineNum;
            std::string lineData;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Uint, "Assembler::Parser", "{}: Expected uint as 1st argument to `map`.", m_CurrentToken.LineNumber);

            startLoc = *m_CurrentToken.Uint;
            
            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Uint, "Assembler::Parser", "{}: Expected uint as 2nd argument to `map`.", m_CurrentToken.LineNumber);

            lineNum = *m_CurrentToken.Uint;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as 3rd argument to `map`.", m_CurrentToken.LineNumber);

            lineData = *m_CurrentToken.Str;

            ProcessEscapeSequences(lineData, m_CurrentToken.LineNumber);

            m_CurrentToken = m_FetchToken();

            DebugLineMapNode lineMap(startLoc, lineNum, lineData);

            lineMap.Line = m_CurrentToken.LineNumber;

            return std::move(lineMap);
        }

        DebugLocalVarNode Parser::ParseDebugLocalVar()
        {
            std::string name, type;
            uint64_t stackOffset;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as 1st argument to `local`.", m_CurrentToken.LineNumber);

            name = *m_CurrentToken.Str;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as 2nd argument to `local`.", m_CurrentToken.LineNumber);

            type = *m_CurrentToken.Str;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Uint, "Assembler::Parser", "{}: Expected uint as 3rd argument to `local`.", m_CurrentToken.LineNumber);

            stackOffset = *m_CurrentToken.Uint;

            DebugLocalVarNode localVar(name, type, stackOffset);

            localVar.Line = m_CurrentToken.LineNumber;

            return std::move(localVar);
        }
        
        DebugRoutineNode Parser::ParseDebugRoutine()
        {
            std::string & name = *m_CurrentToken.Str;

            DebugRoutineNode routine(name, "", 0, 0);

            routine.Line = m_CurrentToken.LineNumber;
            
            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Arbitarory && m_CurrentToken.Arbitarory->Type == ':', "Assembler::Parser", "{}: Expected a `:` after routine `{}`.", m_CurrentToken.LineNumber, name);

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.NewLine, "Assembler::Parser", "{}: Expected newline after `:`.", m_CurrentToken.LineNumber);

            SkipNewlines();

            if (m_CurrentToken.Str && *m_CurrentToken.Str == "META")
            {
                routine.MetaData = std::move(ParseDebugMeta());
            }

            m_CurrentToken = m_FetchToken();

            SkipNewlines();

            while (m_CurrentToken.Str && (*m_CurrentToken.Str == "MAP" || *m_CurrentToken.Str == "LOCAL"))
            {
                if (*m_CurrentToken.Str == "MAP")
                {
                    routine.LineMaps.emplace_back(std::move(ParseDebugLineMap()));
                }
                else
                {
                    routine.LocalVariables.emplace_back(std::move(ParseDebugLocalVar()));
                }

                m_CurrentToken = m_FetchToken();

                SkipNewlines();
            }

            return std::move(routine);

        }

        DebugSectionNode Parser::ParseDebugSection()
        {
            DebugSectionNode dbg;

            dbg.Line = m_CurrentToken.LineNumber;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Str)
            {
                SkipNewlines();

                dbg.Routines.emplace_back(std::move(ParseDebugRoutine()));

                SkipNewlines();
            }

            return std::move(dbg);
        }
        
        ReflectionAttributeNode Parser::ParseReflectionAttribute()
        {
            std::string name;
            std::vector<std::string> attributes;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str after `attribute` in reflection section.", m_CurrentToken.LineNumber);

            name = *m_CurrentToken.Str;

            m_CurrentToken = m_FetchToken();
            
            FLCN_REL_ASSERT(m_CurrentToken.Arbitarory && m_CurrentToken.Arbitarory->Type == '[', "Assembler::Parser", "{}: Expected `[` after target name in `attribute` in reflection section.", m_CurrentToken.LineNumber);

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Str)
            {
                attributes.emplace_back(*m_CurrentToken.Str);

                m_CurrentToken = m_FetchToken();

                SkipNewlines();

                FLCN_REL_ASSERT(
                    m_CurrentToken.Arbitarory && (m_CurrentToken.Arbitarory->Type == ',' || m_CurrentToken.Arbitarory->Type == ']'),
                    "Assembler::Parser", "{}: Expected `,` or `]` after `attribute` in reflection section.", m_CurrentToken.LineNumber
                );
                
                if (m_CurrentToken.Arbitarory->Type == ']')
                {
                    break;
                }

                m_CurrentToken = m_FetchToken();

                SkipNewlines();
            }
            
            m_CurrentToken = m_FetchToken();

            ReflectionAttributeNode attrib(name, attributes);

            attrib.Line = m_CurrentToken.LineNumber;

            return std::move(attrib);
        }

        ReflectionFunctionNode Parser::ParseReflectionFunction()
        {
            std::string name, retType;
            std::vector<std::string> params;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as return type after `function` in reflection section.", m_CurrentToken.LineNumber);

            retType = *m_CurrentToken.Str;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as function name in `function` in reflection section.", m_CurrentToken.LineNumber);

            name = *m_CurrentToken.Str;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Arbitarory && m_CurrentToken.Arbitarory->Type == '(', "Assembler::Parser", "{}: Expected `(` after function name in `function` in reflection section.", m_CurrentToken.LineNumber);

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Str)
            {
                params.emplace_back(*m_CurrentToken.Str);

                m_CurrentToken = m_FetchToken();

                SkipNewlines();

                FLCN_REL_ASSERT(
                    m_CurrentToken.Arbitarory && (m_CurrentToken.Arbitarory->Type == ',' || m_CurrentToken.Arbitarory->Type == ')'),
                    "Assembler::Parser", "{}: Expected `,` or `)` after `function` in reflection section.", m_CurrentToken.LineNumber
                );
                
                if (*m_CurrentToken.Char == ')')
                {
                    break;
                }

                m_CurrentToken = m_FetchToken();

                SkipNewlines();
            }

            m_CurrentToken = m_FetchToken();

            ReflectionFunctionNode function(name, retType, params);

            function.Line = m_CurrentToken.LineNumber;

            return std::move(function);
        }

        ReflectionStructureNode Parser::ParseReflectionStructure()
        {
            std::string name;
            std::vector<std::pair<std::string, std::string>> members;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as struct name in `struct` in reflection section.", m_CurrentToken.LineNumber);

            name = *m_CurrentToken.Str;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Arbitarory->Type && m_CurrentToken.Arbitarory->Type == '{', "Assembler::Parser", "{}: Expected `{` after struct name in `struct` in reflection section.", m_CurrentToken.LineNumber);

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Str)
            {
                members.emplace_back(*m_CurrentToken.Str, "");

                m_CurrentToken = m_FetchToken();

                if (m_CurrentToken.Str)
                {
                    members[members.size() - 1].second = *m_CurrentToken.Str;
                }

                m_CurrentToken = m_FetchToken();

                SkipNewlines();

                FLCN_REL_ASSERT(
                    m_CurrentToken.Arbitarory->Type && (m_CurrentToken.Arbitarory->Type == ',' || m_CurrentToken.Arbitarory->Type == '}'),
                    "Assembler::Parser", "{}: Expected `,` or `}` after member name in `struct` in reflection section.", m_CurrentToken.LineNumber
                );

                if (*m_CurrentToken.Char == '}')
                {
                    m_CurrentToken = m_FetchToken();
                    break;
                }

                m_CurrentToken = m_FetchToken();

                SkipNewlines();
            }

            ReflectionStructureNode structure(name, members);

            structure.Line = m_CurrentToken.LineNumber;

            return std::move(structure);
        }

        ReflectionAliasNode Parser::ParseReflectionAlias()
        {
            std::string name, base;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as alias name in `alias` in reflection section.", m_CurrentToken.LineNumber);

            name = *m_CurrentToken.Str;

            m_CurrentToken = m_FetchToken();

            FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as base type name in `alias` in reflection section.", m_CurrentToken.LineNumber);

            base = *m_CurrentToken.Str;

            ReflectionAliasNode alias(name, base);

            alias.Line = m_CurrentToken.LineNumber;

            return std::move(alias);
        }

        ReflectionSectionNode Parser::ParseReflectionSection()
        {
            ReflectionSectionNode refl;

            refl.Line = m_CurrentToken.LineNumber;

            m_CurrentToken = m_FetchToken();

            while (m_CurrentToken.Str && (*m_CurrentToken.Str == "ATTRIBUTE" || *m_CurrentToken.Str == "FUNCTION" || *m_CurrentToken.Str == "STRUCT" || *m_CurrentToken.Str == "ALIAS"))
            {
                SkipNewlines();

                if (m_CurrentToken.Str == "ATTRIBUTE")
                {
                    refl.Attributes.emplace_back(std::move(ParseReflectionAttribute()));
                }
                else if (m_CurrentToken.Str == "FUNCTION")
                {
                    refl.Functions.emplace_back(std::move(ParseReflectionFunction())); 
                }
                else if(m_CurrentToken.Str == "STRUCT")
                {
                    refl.Structures.emplace_back(std::move(ParseReflectionStructure()));
                }
                else if (m_CurrentToken.Str == "ALIAS")
                {
                    refl.Aliases.emplace_back(std::move(ParseReflectionAlias()));
                }
                
                SkipNewlines();
            }

            return std::move(refl);
        }
        
        ASTNode * Parser::ParseModule()
        {
            ModuleNode * module = new ModuleNode();

            auto checkSyntax = [&](const std::string & sectionName)
            {
                FLCN_REL_ASSERT(m_CurrentToken.Section, "Assembler::Parser", "{}: Expected `sect` at toplevel.", m_CurrentToken.LineNumber);

                m_CurrentToken = m_FetchToken();

                FLCN_REL_ASSERT(m_CurrentToken.Str, "Assembler::Parser", "{}: Expected identifier|str as section name after `sect`.", m_CurrentToken.LineNumber);

                FLCN_REL_ASSERT(*m_CurrentToken.Str == sectionName, "Assembler::Parser", "{}: Invalid section name `{}`.", m_CurrentToken.LineNumber, *m_CurrentToken.Str);

                m_CurrentToken = m_FetchToken();

                FLCN_REL_ASSERT(m_CurrentToken.Arbitarory && m_CurrentToken.Arbitarory->Type == ':', "Assembler::Parser", "{}: Expected a `:` after section name.", m_CurrentToken.LineNumber);

                FLCN_REL_ASSERT((m_CurrentToken = m_FetchToken()).NewLine, "Assembler::Parser", "{}: Expected newline after `:`.", m_CurrentToken.LineNumber);
            };

            SkipNewlines();

            checkSyntax("code");

            module->CodeSection = std::move(ParseCodeSection());

            SkipNewlines();

            checkSyntax("debug");

            module->DebugSection = std::move(ParseDebugSection());

            SkipNewlines();

            checkSyntax("reflection");

            module->ReflectionSection = std::move(ParseReflectionSection());

            return module;
        }

        ASTNode * Parser::Parse()
        {
            return ParseModule();
        }
    }
}
