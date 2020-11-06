/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "Parser.hpp"

namespace Falcon
{
    namespace FCC
    {
        namespace FalconScript
        {
            static bool s_IgnorePrecedence = false;
            static bool s_WaitingForValue = false;

            static std::unordered_map<std::string, bool> s_OperatorIsBinary
            {
                {"+"    , true},
                {"-"    , true},
                {"*"    , true},
                {"/"    , true},
                {"%"    , true},
                {"<<"   , true},
                {">>"   , true},
                {"&"    , true},
                {"|"    , true},
                {"^"    , true},
                {">"    , true},
                {">="   , true},
                {"<"    , true},
                {"<="   , true},
                {"=="   , true},
                {"!="   , true},
                {"&&"   , true},
                {"||"   , true},
                {"="    , true},
                {","    , true},
                {"."    , true},

                {"++"   , false},
                {"--"   , false},
                {"~"    , false},
                {"!"    , false}
            };

            static std::unordered_map<std::string, char> s_OperatorStringToChar
            {
                {"++", 'I'},
                {"--", 'D'},
                {"<<", 'l'},
                {">>", 'r'},
                {">=", '2'},
                {"<=", 'S'},
                {"==", 'q'},
                {"!=", 'n'},
                {"&&", 'a'},
                {"||", 'o'}
            };

            static std::unordered_map<char, size_t> s_OperatorPrecedences
            {
                {'.'    , 0},
                {'r'    , 1},
                {'D'    , 1},
                {'i'    , 1},
                {'d'    , 1},
                {'~'    , 1},
                {'!'    , 1},
                {'l'    , 2},
                {'r'    , 2},
                {'/'    , 3},
                {'%'    , 3},
                {'*'    , 4},
                {'+'    , 5},
                {'-'    , 6},
                {'&'    , 7},
                {'|'    , 7},
                {'^'    , 7},
                {'>'    , 8},
                {'2'    , 8},
                {'<'    , 8},
                {'S'    , 8},
                {'q'    , 8},
                {'n'    , 8},
                {'a'    , 9},
                {'o'    , 9},
                {','    , 10},
                {'@'    , 11},
                {'='    , 12},
                {'R'    , 13}
            };

            template <typename Type>
            inline bool FindInVector(const std::vector<Type> & vec, std::function<bool(const Type &)> condition)
            {
                return std::find_if(vec.begin(), vec.end(), condition) != vec.end();
            }

            Parser::Parser(const std::vector<Token> & tokens)
                : m_Tokens(tokens), m_Cursor(0), m_CurrentModule(nullptr), m_CurrentClass(nullptr), m_CurrentFunction(nullptr)
            {
            }

            Parser::Parser(std::vector<Token> && tokens)
                : m_Tokens(std::move(tokens)), m_Cursor(0), m_CurrentModule(nullptr), m_CurrentClass(nullptr), m_CurrentFunction(nullptr)
            {
            }

            void Parser::AddValue(FALI::ValueNode & root, const FALI::ValueNode & value)
            {
                FLCN_REL_ASSERT(!root.Literal && !root.Variable, "Parser", "{}: Expected an operator, got a value.", Current().LineNumber);

                if (root.Operator)
                {
                    if ((*root.Operator)->Binary)
                    {
                        if (!(*root.Operator)->Right)
                        {
                            (*root.Operator)->Right = value;
                            (*root.Operator)->Right->LineNumber = Current().LineNumber;
                            s_WaitingForValue = false;
                        }
                        else
                        {
                            AddValue(*(*root.Operator)->Right, value);
                        }
                    }
                    else
                    {
                        if (!(*root.Operator)->Left)
                        {
                            (*root.Operator)->Left = value;
                            (*root.Operator)->Left->LineNumber = Current().LineNumber;
                            s_WaitingForValue = false;
                        }
                        else
                        {
                            AddValue(*(*root.Operator)->Left, value);
                        }
                    }
                }
                else
                {
                    root = value;
                    root.LineNumber = Current().LineNumber;
                }
            }

            void Parser::AddUnaryOperator(FALI::ValueNode & root, char opType)
            {
                if (root.Literal || root.Variable)
                {
                    root = FALI::ValueNode(new FALI::OperatorNode(opType, root, Current().LineNumber), Current().LineNumber);
                }
                else if (root.Operator)
                {
                    if ((*root.Operator)->Binary)
                    {
                        if (!(*root.Operator)->Right)
                        {
                            (*root.Operator)->Right = FALI::ValueNode(new FALI::OperatorNode(opType, Current().LineNumber), Current().LineNumber);
                            s_WaitingForValue = true;
                        }
                        else if ((*root.Operator)->Right->Literal || (*root.Operator)->Right->Variable)
                        {
                            (*root.Operator)->Right = FALI::ValueNode(new FALI::OperatorNode(opType, *(*root.Operator)->Right, Current().LineNumber), Current().LineNumber);
                        }
                        else if ((*root.Operator)->Right->Operator)
                        {
                            if ((*(*root.Operator)->Left->Operator)->Binary)
                            {
                                AddBinaryOperator(*(*root.Operator)->Right, opType);
                            }
                            else
                            {
                                AddUnaryOperator(*(*root.Operator)->Right, opType);
                            }
                        }
                    }
                    else if ((*root.Operator)->Left)
                    {
                        (*root.Operator)->Left = FALI::ValueNode(new FALI::OperatorNode(opType, *(*root.Operator)->Left, Current().LineNumber), Current().LineNumber);
                    }
                    else
                    {
                        (*root.Operator)->Right = FALI::ValueNode(new FALI::OperatorNode(opType, Current().LineNumber), Current().LineNumber);
                        s_WaitingForValue = true;
                    }
                }
                else
                {
                    root = FALI::ValueNode(new FALI::OperatorNode(opType, FALI::ValueNode(), Current().LineNumber), Current().LineNumber);
                }
            }

            void Parser::AddBinaryOperator(FALI::ValueNode & root, char opType)
            {
                if (root.Literal || root.Variable)
                {
                    root = FALI::ValueNode(new FALI::OperatorNode(opType, root, Current().LineNumber), Current().LineNumber);
                    (*root.Operator)->Binary = true;
                    s_WaitingForValue = true;
                }
                else if (root.Operator)
                {
                    if ((*root.Operator)->Binary)
                    {
                        if (s_OperatorPrecedences[opType] <= s_OperatorPrecedences[(*root.Operator)->Type] && (!s_IgnorePrecedence || s_OperatorPrecedences[(*root.Operator)->Type] > 8))
                        {
                            if (!(*root.Operator)->Right)
                            {
                                FLCN_REL_ASSERT(false, "Parser", "{}: Expected value, got binary operator.", Current().LineNumber);
                            }
                            else if ((*root.Operator)->Right->Literal || (*root.Operator)->Right->Variable)
                            {
                                (*root.Operator)->Right = FALI::ValueNode(new FALI::OperatorNode(opType, *(*root.Operator)->Right, Current().LineNumber), Current().LineNumber);
                                (*(*root.Operator)->Right->Operator)->Binary = true;
                                s_WaitingForValue = true;
                            }
                            else if ((*root.Operator)->Right->Operator)
                            {
                                if ((*(*root.Operator)->Right->Operator)->Binary)
                                {
                                    AddBinaryOperator(*(*root.Operator)->Right, opType);
                                }
                                else
                                {
                                    AddUnaryOperator(*(*root.Operator)->Right, opType);
                                }
                            }
                        }
                        else
                        {
                            if (s_IgnorePrecedence)
                            {
                                s_IgnorePrecedence = false;
                            }

                            root = FALI::ValueNode(new FALI::OperatorNode(opType, root, Current().LineNumber), Current().LineNumber);
                            (*root.Operator)->Binary = true;
                            s_WaitingForValue = true;
                        }
                    }
                    else
                    {
                        if (s_OperatorPrecedences[opType] <= s_OperatorPrecedences[(*root.Operator)->Type] && (!s_IgnorePrecedence || s_OperatorPrecedences[(*root.Operator)->Type] > 8))
                        {
                            if (!(*root.Operator)->Left)
                            {
                                FLCN_REL_ASSERT(false, "Parser", "{}: Expected value, got binary operator.", Current().LineNumber);
                            }
                            else if ((*root.Operator)->Left->Literal || (*root.Operator)->Left->Variable)
                            {
                                (*root.Operator)->Left = FALI::ValueNode(new FALI::OperatorNode(opType, *(*root.Operator)->Left, Current().LineNumber), Current().LineNumber);
                                (*(*root.Operator)->Left->Operator)->Binary = true;
                                s_WaitingForValue = true;
                            }
                            else if ((*root.Operator)->Left->Operator)
                            {
                                if ((*(*root.Operator)->Left->Operator)->Binary)
                                {
                                    AddBinaryOperator(*(*root.Operator)->Left, opType);
                                }
                                else
                                {
                                    AddUnaryOperator(*(*root.Operator)->Left, opType);
                                }
                            }
                        }
                        else
                        {
                            if (s_IgnorePrecedence)
                            {
                                s_IgnorePrecedence = false;
                            }

                            root = FALI::ValueNode(new FALI::OperatorNode(opType, root, Current().LineNumber), Current().LineNumber);
                            (*root.Operator)->Binary = true;
                            s_WaitingForValue = true;
                        }
                    }
                }
                else
                {
                    FLCN_REL_ASSERT(false, "Parser", "{}: Expected value, got binary operator.", Current().LineNumber);
                }
            }

            bool Parser::IsType()
            {
                size_t cursorBackUp = m_Cursor;

                FALI::DataType type{"", 0};

                type.Flags |= ParseTypeFlags();

                if (type.Flags != 0)
                {
                    Advance();
                }

                if (!Current().Word)
                {
                    m_Cursor = cursorBackUp;
                    return false;
                }

                bool expectWord = true;

                while (expectWord ? Current().Word.has_value() : Current().Operator.has_value())
                {
                    if (Current().Operator && Current().Operator->Type == ".")
                    {
                        type.Name += '.';
                        expectWord = true;
                    }
                    else if (Current().Word)
                    {
                        type.Name += Current().Word->Str;
                        expectWord = false;
                    }
                    else
                    {
                        break;
                    }

                    Advance();
                }

                if (expectWord)
                {
                    m_Cursor = cursorBackUp;
                    return false;
                }

                if (FALI::s_Types.count(type.Name) == 0)
                {
                    m_Cursor = cursorBackUp;
                    return false;
                }

                FALI::TypeFlag suffixFlags = ParseTypeFlags();

                if (suffixFlags != 0)
                {
                    type.Flags |= suffixFlags;
                }

                m_Cursor = cursorBackUp;

                return true;
            }

            FALI::TypeFlag Parser::ParseTypeFlags()
            {
                FALI::TypeFlag flags = 0;

                while (Current().Word)
                {
                    if (Current().Word->Str == "const")
                    {
                        flags |= FALI::TypeFlags::CONST;
                    }
                    else if (Current().Word->Str == "tmp")
                    {
                        flags |= FALI::TypeFlags::TEMPARORY;
                    }
                    else if (Current().Word->Str == "ref")
                    {
                        flags |= FALI::TypeFlags::REFERENCE;
                    }
                    else
                    {
                        break;
                    }

                    Advance();
                }

                if (flags != 0)
                {
                    m_Cursor--;
                }

                return flags;
            }

            FALI::DataType Parser::ParseType()
            {
                FALI::DataType type{"", 0};

                type.Flags |= ParseTypeFlags();

                if (type.Flags != 0)
                {
                    Advance();
                }

                FLCN_REL_ASSERT(Current().Word, "Parser", "{}: Expected type name.", Current().LineNumber);

                bool expectWord = true;

                while (expectWord ? Current().Word.has_value() : Current().Operator.has_value())
                {
                    if (Current().Operator && Current().Operator->Type == ".")
                    {
                        type.Name += '.';
                        expectWord = true;
                    }
                    else if (Current().Word)
                    {
                        type.Name += Current().Word->Str;
                        expectWord = false;
                    }
                    else
                    {
                        break;
                    }

                    Advance();
                }

                FLCN_REL_ASSERT(!expectWord, "Parser", "{}: Expected type after '.' in type name.", Current().LineNumber);

                FLCN_REL_ASSERT(FALI::s_Types.count(type.Name) == 1, "Parser", "{}: Expected valid type name, got '{}'.", Current().LineNumber, type.Name);

                FALI::TypeFlag suffixFlags = ParseTypeFlags();

                if (suffixFlags != 0)
                {
                    type.Flags |= suffixFlags;
                    m_Cursor--;
                }

                return std::move(type);
            }

            FALI::IfNode Parser::ParseIfNode()
            {
                FLCN_REL_ASSERT(Advance().Punctuator->Type == '(', "Parser", "{}: Expected a '('.", Current().LineNumber);

                FALI::IfNode ifNode(std::move(ParseExpression(')')));
                ifNode.LineNumber = Current().LineNumber;

                FLCN_REL_ASSERT(Advance().Punctuator->Type == '{', "Parser", "{}: Expected a '{'.", Current().LineNumber);

                bool lastStatementWasIf = false;

                while (true)
                {
                    Token & token = Advance();

                    if (token.Punctuator)
                    {
                        if (token.Punctuator->Type == '}')
                        {
                            break;
                        }
                        else
                        {
                            m_Cursor--;
                        }
                    }
                    else
                    {
                        m_Cursor--;
                    }

                    ifNode.Statements.emplace_back(std::move(ParseStatement(lastStatementWasIf)));

                    if (ifNode.Statements.back().IfBlock)
                    {
                        lastStatementWasIf = true;
                    }
                    else
                    {
                        lastStatementWasIf = false;
                    }
                }

                return ifNode;
            }

            FALI::ElseNode Parser::ParseElseNode()
            {
                FALI::ElseNode elseNode;
                elseNode.LineNumber = Current().LineNumber;

                FLCN_REL_ASSERT(Advance().Punctuator->Type == '{', "Parser", "{}: Expected a '{'.", Current().LineNumber);

                bool lastStatementWasIf = false;

                while (true)
                {
                    Token & token = Advance();

                    if (token.Punctuator)
                    {
                        if (token.Punctuator->Type == '}')
                        {
                            break;
                        }
                        else
                        {
                            m_Cursor--;
                        }
                    }
                    else
                    {
                        m_Cursor--;
                    }

                    elseNode.Statements.emplace_back(std::move(ParseStatement(lastStatementWasIf)));

                    if (elseNode.Statements.back().IfBlock)
                    {
                        lastStatementWasIf = true;
                    }
                    else
                    {
                        lastStatementWasIf = false;
                    }
                }

                return elseNode;
            }

            FALI::WhileNode Parser::ParseWhileNode()
            {
                FLCN_REL_ASSERT(Advance().Punctuator->Type == '(', "Parser", "{}: Expected a '('.", Current().LineNumber);

                FALI::WhileNode whileNode(std::move(ParseExpression(')')), Current().LineNumber);

                FLCN_REL_ASSERT(Advance().Punctuator->Type == '{', "Parser", "{}: Expected a '{'.", Current().LineNumber);

                bool lastStatementWasIf = false;

                while (true)
                {
                    Advance();

                    if (Current().Punctuator)
                    {
                        if (Current().Punctuator->Type == '}')
                        {
                            break;
                        }
                        else
                        {
                            m_Cursor--;
                        }
                    }
                    else
                    {
                        m_Cursor--;
                    }

                    whileNode.Statements.emplace_back(std::move(ParseStatement(lastStatementWasIf)));

                    if (whileNode.Statements.back().IfBlock)
                    {
                        lastStatementWasIf = true;
                    }
                    else
                    {
                        lastStatementWasIf = false;
                    }
                }

                return whileNode;
            }

            FALI::OperatorNode Parser::ParseExpression(char terminator)
            {
                FALI::ValueNode value;

                Advance();

                bool lastWaitingForValue = s_WaitingForValue;
                s_WaitingForValue = false;

                bool functionCall = false;

                while(!(Current().Punctuator && Current().Punctuator->Type == terminator))
                {
                    if (Current().Whole)
                    {
                        AddValue(value, FALI::ValueNode(FALI::LiteralNode(*Current().Whole, Current().LineNumber), Current().LineNumber));
                    }
                    else if (Current().Integer)
                    {
                        AddValue(value, FALI::ValueNode(FALI::LiteralNode(*Current().Integer, Current().LineNumber), Current().LineNumber));
                    }
                    else if (Current().Real)
                    {
                        AddValue(value, FALI::ValueNode(FALI::LiteralNode(*Current().Real, Current().LineNumber), Current().LineNumber));
                    }
                    else if (Current().Word)
                    {
                        if (Current().Word->Str == "return")
                        {
                            AddUnaryOperator(value, 'R');
                        }
                        else if (Current().Word->Str == "addr")
                        {
                            AddUnaryOperator(value, 'r');
                        }
                        else if (Current().Word->Str == "deref")
                        {
                            Advance();

                            FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == '(', "Parser", "{}: Expected '(' after 'deref'.", Current().LineNumber);

                            Advance();

                            FALI::DataType toType = std::move(ParseType());

                            AddValue(value, FALI::ValueNode(FALI::VariableNode("", toType, Current().LineNumber), Current().LineNumber));

                            Advance();

                            FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == ')', "Parser", "{}: Expected ')' after 'deref(<type>'.", Current().LineNumber);

                            AddBinaryOperator(value, 'D');
                        }
                        else if (IsType())
                        {
                            FALI::DataType type = std::move(ParseType());

                            if (Current().Word)
                            {
                                std::string name = Current().Word->Str;

                                m_CurrentFunction->LocalVariables[name] = FALI::VariableNode(name, type, Current().LineNumber);

                                AddValue(value, FALI::ValueNode(m_CurrentFunction->LocalVariables[name], Current().LineNumber));
                            }
                            else if (Current().Punctuator && Current().Punctuator->Type == '(')
                            {
                                AddValue(value, FALI::ValueNode(FALI::VariableNode(type.Name, "", 0, Current().LineNumber), Current().LineNumber));

                                AddBinaryOperator(value, '@');

                                functionCall = true;
                            }
                            else
                            {
                                FLCN_REL_ASSERT(false, "Parser", "{}: Invalid declaration.", Current().LineNumber);
                            }
                        }
                        else
                        {
                            Advance();

                            if (Current().Punctuator && Current().Punctuator->Type == '(')
                            {
                                m_Cursor--;

                                AddValue(value, FALI::ValueNode(FALI::VariableNode(Current().Word->Str, "", 0, Current().LineNumber), Current().LineNumber));

                                Advance();

                                AddBinaryOperator(value, '@');

                                functionCall = true;
                            }
                            else
                            {
                                m_Cursor--;

                                auto condition = [&](const FALI::VariableNode & other)->bool
                                {
                                    if (other.Name == Current().Word->Str)
                                    {
                                        return true;
                                    }
                                    else
                                    {
                                        return false;
                                    }
                                };

                                std::vector<FALI::VariableNode>::iterator inPublicVars, inProtectedVars, inPrivateVars;

                                if (m_CurrentClass)
                                {
                                    inPublicVars = std::find_if(m_CurrentClass->PublicMemberVariables.begin(), m_CurrentClass->PublicMemberVariables.end(), condition);
                                    inProtectedVars = std::find_if(m_CurrentClass->ProtectedMemberVariables.begin(), m_CurrentClass->ProtectedMemberVariables.end(), condition);
                                    inPrivateVars = std::find_if(m_CurrentClass->PrivateMemberVariables.begin(), m_CurrentClass->PrivateMemberVariables.end(), condition);
                                }

                                if (m_CurrentFunction->LocalVariables.count(Current().Word->Str) == 1)
                                {
                                    AddValue(value, FALI::ValueNode(FALI::VariableNode(Current().Word->Str, m_CurrentFunction->LocalVariables[Current().Word->Str].Type, Current().LineNumber), Current().LineNumber));
                                }
                                else if (m_CurrentClass && inPublicVars != m_CurrentClass->PublicMemberVariables.end())
                                {
                                    AddValue(value, FALI::ValueNode(FALI::VariableNode(Current().Word->Str, inPublicVars->Type, Current().LineNumber), Current().LineNumber));
                                }
                                else if (m_CurrentClass && inProtectedVars != m_CurrentClass->ProtectedMemberVariables.end())
                                {
                                    AddValue(value, FALI::ValueNode(FALI::VariableNode(Current().Word->Str, inProtectedVars->Type, Current().LineNumber), Current().LineNumber));
                                }
                                else if (m_CurrentClass && inPrivateVars != m_CurrentClass->PrivateMemberVariables.end())
                                {
                                    AddValue(value, FALI::ValueNode(FALI::VariableNode(Current().Word->Str, inPrivateVars->Type, Current().LineNumber), Current().LineNumber));
                                }
                                else
                                {
                                    AddValue(value, FALI::ValueNode(FALI::VariableNode(Current().Word->Str, "___<invalid>___", 0, Current().LineNumber), Current().LineNumber));
                                }
                            }
                        }
                    }
                    else if (Current().Operator)
                    {
                        if (s_OperatorIsBinary[Current().Operator->Type])
                        {
                            AddBinaryOperator(value, Current().Operator->Type.size() == 1 ? Current().Operator->Type[0] : s_OperatorStringToChar[Current().Operator->Type]);
                        }
                        else
                        {
                            AddUnaryOperator(value, Current().Operator->Type.size() == 1 ? Current().Operator->Type[0] : s_OperatorStringToChar[Current().Operator->Type]);
                        }
                    }
                    else if (Current().Punctuator)
                    {
                        if (Current().Punctuator->Type == '(' && !functionCall)
                        {
                            AddValue(value, FALI::ValueNode(new FALI::OperatorNode(ParseExpression(')')), Current().LineNumber));
                            s_IgnorePrecedence = true;
                        }
                        else if (Current().Punctuator->Type == ',')
                        {
                            AddBinaryOperator(value, ',');
                        }
                        else if (Current().Punctuator->Type == ')')
                        {
                            if (s_WaitingForValue)
                            {
                                AddValue(value, FALI::ValueNode());
                            }

                            functionCall = false;
                        }
                    }

                    Advance();
                }

                FLCN_REL_ASSERT(!s_WaitingForValue, "Parser", "{}: Expected value.", Current().LineNumber);

                s_WaitingForValue = lastWaitingForValue;

                return std::move(**value.Operator);
            }

            FALI::StatementNode Parser::ParseStatement(bool lastStatementWasIf)
            {
                Advance();

                uint64_t line = Current().LineNumber;

                if (Current().Word && Current().Word->Str == "if")
                {
                    FALI::StatementNode statement(ParseIfNode(), line);
                    
                    return std::move(statement);
                }
                else if (Current().Word && Current().Word->Str == "else")
                {
                    FLCN_REL_ASSERT(lastStatementWasIf, "Parser", "{}: Unexpected 'else'. 'else' must appear after 'if' block.", Current().LineNumber);

                    FALI::StatementNode statement(ParseElseNode(), line);

                    return std::move(statement);
                }
                else if (Current().Word && Current().Word->Str == "while")
                {
                    FALI::StatementNode statement(ParseWhileNode(), line);

                    return std::move(statement);
                }
                else
                {
                    m_Cursor--;

                    FALI::StatementNode statement(ParseExpression(), line);

                    return std::move(statement);
                }
            }

            FALI::FunctionNode Parser::ParseFunction(bool member, const std::string & className, bool specialMember, const std::string & specialMemberName)
            {
                if (!specialMember)
                {
                    Advance();
                }

                FALI::DataType returnType = !specialMember ? std::move(ParseType()) : FALI::DataType{"void", 0};

                FLCN_REL_ASSERT(Current().Word || specialMember, "Parser", "{}: Expected function name after return type.", Current().LineNumber);

                uint64_t line = Current().LineNumber;

                std::string name = !specialMember ? Current().Word->Str : specialMemberName;

                std::vector<std::pair<FALI::DataType, std::string>> params;

                if (member)
                {
                    params.emplace_back(FALI::DataType{className, FALI::TypeFlags::REFERENCE}, "this");
                }

                Advance();

                FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == '(', "Parser", "{}: Expected a '('.", Current().LineNumber);

                while (Advance().Word)
                {
                    FALI::DataType paramType = std::move(ParseType());

                    std::string paramName = Current().Word->Str;

                    params.emplace_back(std::move(paramType), std::move(paramName));

                    Advance();

                    FLCN_REL_ASSERT(Current().Punctuator, "Parser", "{}: Expected ',' or ')'.", Current().LineNumber);

                    char punct = Current().Punctuator->Type;

                    FLCN_REL_ASSERT(punct == ',' || punct == ')', "Parser", "{}: Expected ',' or ')'.", Current().LineNumber);

                    if (punct == ')')
                    {
                        break;
                    }
                }

                FALI::FunctionNode function(returnType, member ? name : m_CurrentNamespace + name, params, line);

                m_CurrentFunction = &function;

                Advance();

                FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == '{', "Parser", "{}: Expected a '{'.", Current().LineNumber);

                bool lastStatementWasIf = false;

                while (true)
                {
                    Advance();

                    if (Current().Punctuator && Current().Punctuator->Type == '}')
                    {
                        break;
                    }
                    else
                    {
                        m_Cursor--;
                    }

                    function.Statements.emplace_back(std::move(ParseStatement(lastStatementWasIf)));

                    if (function.Statements.back().IfBlock)
                    {
                        lastStatementWasIf = true;
                    }
                    else
                    {
                        lastStatementWasIf = false;
                    }
                }

                return std::move(function);
            }

            FALI::ClassNode Parser::ParseClass()
            {
                FLCN_REL_ASSERT(Advance().Word, "Parser", "{}: Expected class name after 'class'.", Current().LineNumber);

                uint64_t line = Current().LineNumber;
                FALI::ClassNode clas(m_CurrentNamespace + Current().Word->Str, line);
                m_CurrentClass = &clas;

                Advance();

                FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == '{', "Parser", "{}: Expected a '{'.", Current().LineNumber);

                Advance();

                std::string accessType = "public";

                FALI::s_Types[clas.Name] = 0;

                while (!Current().Punctuator || Current().Punctuator->Type != '}')
                {
                    if (Current().Word && Current().Word->Str == "public")
                    {
                        Advance();

                        FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == ':', "Parser", "{}: Expected a ':' after access specifier.", Current().LineNumber);

                        accessType = "public";
                    }
                    else if (Current().Word && Current().Word->Str == "protected")
                    {
                        Advance();

                        FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == ':', "Parser", "{}: Expected a ':' after access specifier.", Current().LineNumber);

                        accessType = "protected";
                    }
                    else if (Current().Word && Current().Word->Str == "private")
                    {
                        Advance();

                        FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == ':', "Parser", "{}: Expected a ':' after access specifier.", Current().LineNumber);

                        accessType = "private";
                    }
                    else if (Current().Word && Current().Word->Str == "constructor")
                    {
                        if (accessType == "public")
                        {
                            clas.PublicMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                        else if (accessType == "protected")
                        {
                            clas.ProtectedMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                        else if (accessType == "private")
                        {
                            clas.PrivateMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                    }
                    else if (Current().Word && Current().Word->Str == "destructor")
                    {
                        if (accessType == "public")
                        {
                            clas.PublicMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___destructor___")));
                        }
                        else if (accessType == "protected")
                        {
                            clas.ProtectedMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___destructor___")));
                        }
                        else if (accessType == "private")
                        {
                            clas.PrivateMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___destructor___")));
                        }
                    }
                    else if (Current().Word && Current().Word->Str == "copy")
                    {
                        if (accessType == "public")
                        {
                            clas.PublicMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                        else if (accessType == "protected")
                        {
                            clas.ProtectedMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                        else if (accessType == "private")
                        {
                            clas.PrivateMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                    }
                    else if (Current().Word && Current().Word->Str == "move")
                    {
                        if (accessType == "public")
                        {
                            clas.PublicMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                        else if (accessType == "protected")
                        {
                            clas.ProtectedMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                        else if (accessType == "private")
                        {
                            clas.PrivateMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name, true, "___constructor___")));
                        }
                    }
                    else if (Current().Word && Current().Word->Str == "oneone")
                    {
                        Advance();

                        FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == ';', "Parser", "{}: Expected a ';' after 'oneone'.", Current().LineNumber);

                        std::vector<std::pair<FALI::DataType, std::string>> params;

                        params.emplace_back(FALI::DataType{clas.Name, FALI::TypeFlags::REFERENCE}, "this");

                        std::transform(clas.PublicMemberVariables.begin(), clas.PublicMemberVariables.end(), params.begin(),
                            [](const FALI::VariableNode & member)->std::pair<FALI::DataType, std::string>
                            {
                                return {member.Type, "_" + member.Name};
                            }
                        );

                        std::transform(clas.ProtectedMemberVariables.begin(), clas.ProtectedMemberVariables.end(), params.begin(),
                            [](const FALI::VariableNode & member)->std::pair<FALI::DataType, std::string>
                            {
                                return {member.Type, "_" + member.Name};
                            }
                        );

                        std::transform(clas.PrivateMemberVariables.begin(), clas.PrivateMemberVariables.end(), params.begin(),
                            [](const FALI::VariableNode & member)->std::pair<FALI::DataType, std::string>
                            {
                                return {member.Type, "_" + member.Name};
                            }
                        );

                        FALI::FunctionNode function("void", "___constructor___", params, Current().LineNumber);

                        for (auto iter = params.begin() + 1; iter != params.end(); ++iter)
                        {
                            function.Statements.emplace_back(
                                FALI::StatementNode(
                                    FALI::OperatorNode('=',
                                        FALI::ValueNode(FALI::VariableNode(iter->second.c_str() + 1, iter->first)),
                                        FALI::ValueNode(FALI::VariableNode(iter->second, iter->first))
                                    )
                                )
                            );
                        }

                        if (accessType == "public")
                        {
                            clas.PublicMemberFunctions.emplace_back(std::move(function));
                        }
                        else if (accessType == "protected")
                        {
                            clas.ProtectedMemberFunctions.emplace_back(std::move(function));
                        }
                        else if (accessType == "private")
                        {
                            clas.PrivateMemberFunctions.emplace_back(std::move(function));
                        }
                    }
                    else
                    {
                        FALI::DataType type = std::move(ParseType());
                        std::string name = Current().Word->Str;

                        Advance();

                        if (Current().Punctuator && Current().Punctuator->Type == '(')
                        {
                            m_Cursor -= 3;

                            if (accessType == "public")
                            {
                                clas.PublicMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name)));
                            }
                            else if (accessType == "protected")
                            {
                                clas.ProtectedMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name)));
                            }
                            else if (accessType == "private")
                            {
                                clas.PrivateMemberFunctions.emplace_back(std::move(ParseFunction(true, clas.Name)));
                            }
                        }
                        else
                        {
                            FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == ';', "Parser", "{}: Expected a ';' at the end of member variable declaration.", Current().LineNumber);

                            if (accessType == "public")
                            {
                                clas.PublicMemberVariables.emplace_back(FALI::VariableNode(name, type, Current().LineNumber));
                            }
                            else if (accessType == "protected")
                            {
                                clas.ProtectedMemberVariables.emplace_back(FALI::VariableNode(name, type, Current().LineNumber));
                            }
                            else if (accessType == "private")
                            {
                                clas.PrivateMemberVariables.emplace_back(FALI::VariableNode(name, type, Current().LineNumber));
                            }
                        }
                    }

                    Advance();
                }

                size_t size = 0;

                for (auto & member : clas.PublicMemberVariables)
                {
                    size += member.Type.GetSize();
                }

                for (auto & member : clas.ProtectedMemberVariables)
                {
                    size += member.Type.GetSize();
                }

                for (auto & member : clas.PrivateMemberVariables)
                {
                    size += member.Type.GetSize();
                }

                FALI::s_Types[clas.Name] = size;

                return std::move(clas);
            }

            void Parser::ParseNamespace()
            {
                FLCN_REL_ASSERT(Advance().Word, "Parser", "{}: Expected namespace name after 'namespace'.", Current().LineNumber);

                std::string lastNamespace = m_CurrentNamespace;

                m_CurrentNamespace += Current().Word->Str + ".";
                
                Advance();

                FLCN_REL_ASSERT(Current().Punctuator && Current().Punctuator->Type == '{', "Parser", "{}: Expected a '{'.", Current().LineNumber);

                Advance();

                while (!Current().Punctuator || Current().Punctuator->Type != '}')
                {
                    if (Current().Word && Current().Word->Str == "namespace")
                    {
                        ParseNamespace();
                    }
                    else if (Current().Word && Current().Word->Str == "class")
                    {
                        m_CurrentModule->Classes.emplace_back(std::move(ParseClass()));
                    }
                    else
                    {
                        m_Cursor--;
                        m_CurrentModule->Functions.emplace_back(std::move(ParseFunction()));
                    }

                    Advance();
                }

                m_CurrentClass = 0;

                m_CurrentNamespace = std::move(lastNamespace);
            }

            FALI::ModuleNode Parser::ParseModule()
            {
                FALI::ModuleNode module;

                m_CurrentModule = &module;

                while (Advance().Valid())
                {
                    if (Current().Word && Current().Word->Str == "namespace")
                    {
                        ParseNamespace();
                    }
                    else if (Current().Word && Current().Word->Str == "class")
                    {
                        module.Classes.emplace_back(std::move(ParseClass()));
                    }
                    else
                    {
                        m_Cursor--;
                        module.Functions.emplace_back(std::move(ParseFunction()));
                    }
                }

                return std::move(module);
            }

            FALI::ModuleNode Parser::Parse()
            {
                return std::move(ParseModule());
            }
        }
    }
}
