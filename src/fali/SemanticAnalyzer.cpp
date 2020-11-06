/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "SemanticAnalyzer.hpp"

namespace Falcon
{
    namespace FALI
    {
        template <typename Type>
        inline bool FindInVector(const std::vector<Type> & vec, std::function<bool(const Type &)> condition)
        {
            return std::find_if(vec.begin(), vec.end(), condition) != vec.end();
        }

        SemanticAnalyzer::SemanticAnalyzer(ASTNode * ast)
            : m_Failed(false), m_AST(ast), m_CurrentClass(nullptr), m_CurrentFunction(nullptr)
        {
            CollectSemanticData();
        }

        void SemanticAnalyzer::CollectSemanticData()
        {
            if (auto module = dynamic_cast<ModuleNode *>(m_AST))
            {
                for (auto & function : module->Functions)
                {
                    m_FunctionData[function.GetSignature()] = &function;
                }

                for (auto & clas : module->Classes)
                {
                    m_ClassData[clas.Name] = &clas;

                    for (auto & function : clas.PublicMemberFunctions)
                    {
                        m_FunctionData[clas.Name + "." + function.GetSignature()] = &function;
                    }

                    for (auto & function : clas.ProtectedMemberFunctions)
                    {
                        m_FunctionData[clas.Name + "." + function.GetSignature()] = &function;
                    }

                    for (auto & function : clas.PrivateMemberFunctions)
                    {
                        m_FunctionData[clas.Name + "." + function.GetSignature()] = &function;
                    }
                }
            }
        }

        DataType SemanticAnalyzer::DeclType(ValueNode * value)
        {
            DataType type;

            if (value->Literal)
            {
                type.Flags = TypeFlags::TEMPARORY;

                if (value->Literal->Boolean)
                {
                    type.Name = "bool";
                }
                else if (value->Literal->Whole)
                {
                    type.Name = "uint32";
                }
                else if (value->Literal->Integer)
                {
                    type.Name = "int32";
                }
                else if (value->Literal->Real)
                {
                    type.Name = "float32";
                }
            }
            else if (value->Variable)
            {
                type = value->Variable->Type;
            }
            else if (value->Operator)
            {
                type = std::move(DeclType(&value->Operator.value()->Left.value()));
            }

            return std::move(type);
        }

        bool SemanticAnalyzer::IsConvertible(const DataType & from, const DataType & to)
        {
            if (from.Name == to.Name && from.Flags == to.Flags)
            {
                return true;
            }

            if (from.Name == to.Name)
            {
                if (from.Flags & TypeFlags::TEMPARORY && to.Flags == 0)
                {
                    return true;
                }

                if (from.Flags == 0  && to.Flags & TypeFlags::TEMPARORY)
                {
                    return true;
                }

                if (from.Flags == 0 && to.Flags == TypeFlags::REFERENCE)
                {
                    return true;
                }
            }

            return false;
        }

        void SemanticAnalyzer::AnalyzeVariable(VariableNode * var)
        {
            auto condition = [var](const VariableNode & other)->bool
            {
                if (other.Name == var->Name)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            };

            if (var->Type.Name == "" || var->Name == "***stack_allocated_ref***")
            {
                return;
            }

            if (m_CurrentFunction && m_CurrentFunction->LocalVariables.count(var->Name) == 0)
            {
                if (m_CurrentClass
                    && (!FindInVector<VariableNode>(m_CurrentClass->PublicMemberVariables, condition)
                    && !FindInVector<VariableNode>(m_CurrentClass->ProtectedMemberVariables, condition)
                    && !FindInVector<VariableNode>(m_CurrentClass->PrivateMemberVariables, condition)))
                {
                    FLCN_REL_ASSERT(false, "SemanticAnalyzer", "{}: Undefined variable '{}' in function '{}'.", var->LineNumber, var->Name, m_CurrentClass->Name + "." + m_CurrentFunction->Name);
                }
                else if (!m_CurrentClass)
                {
                    FLCN_REL_ASSERT(false, "SemanticAnalyzer", "{}: Undefined variable '{}' in function '{}'.", var->LineNumber, var->Name, m_CurrentFunction->Name);
                }
            }
        }

        void SemanticAnalyzer::AnalyzeOperator(OperatorNode * op)
        {
            if (op->Type == '@')
            {
                std::string functionName;

                if (op->Left->Variable)
                {
                    if (m_ClassData.count(op->Left->Variable->Name) == 1)
                    {
                        op->Left->Variable->Name += ".___constructor___";
                    }

                    functionName = op->Left->Variable->Name;

                    if (op->Right)
                    {
                        op->Right = ValueNode(new OperatorNode(',', ValueNode(VariableNode("***stack_allocated_ref***", m_ClassData[functionName.substr(0, functionName.size() - 18)]->Name, TypeFlags::REFERENCE, op->LineNumber), op->LineNumber), op->Right.value(), op->LineNumber));
                    }
                    else
                    {
                        op->Right = ValueNode(VariableNode("***stack_allocated_ref***", m_ClassData[functionName.substr(0, functionName.size() - 18)]->Name, TypeFlags::REFERENCE, op->LineNumber), op->LineNumber);
                    }
                }
                else if (op->Left->Operator)
                {
                    std::function<std::string(OperatorNode &)> getCallString = [&](OperatorNode & op)->std::string
                    {
                        std::string str;

                        str += op.Left->Variable->Type.Name + ".";
                        
                        if (op.Right->Operator)
                        {
                            str += getCallString(*op.Right->Operator.value());
                        }
                        else if (op.Right->Variable)
                        {
                            str += op.Right->Variable->Name;
                        }

                        return std::move(str);
                    };

                    std::function<std::string(OperatorNode &)> getThis = [&](OperatorNode & op)->std::string
                    {
                        std::string str;

                        str += op.Left->Variable->Name + ".";
                        
                        if (op.Right->Operator)
                        {
                            str += getThis(*op.Right->Operator.value());
                        }
                        else if (op.Right->Variable)
                        {
                            str += op.Right->Variable->Name;
                        }

                        return std::move(str);
                    };

                    functionName = std::move(getCallString(*op->Left->Operator.value()));

                    std::string thisName = getThis(*op->Left->Operator.value());

                    if (op->Right && (op->Right->Literal || op->Right->Variable || op->Right->Operator))
                    {
                        op->Right = ValueNode(new OperatorNode(',', ValueNode(VariableNode(thisName, DeclType(&op->Left.value()).Name, TypeFlags::REFERENCE, op->LineNumber), op->LineNumber), op->Right.value(), op->LineNumber));
                    }
                    else
                    {
                        op->Right = ValueNode(VariableNode(getThis(*op->Left->Operator.value()), DeclType(&op->Left.value()).Name, TypeFlags::REFERENCE, op->LineNumber), op->LineNumber);
                    }
                }

                std::function<std::vector<DataType>(OperatorNode & op)> getArgs = [&](OperatorNode & op)->std::vector<DataType>
                {
                    std::vector<DataType> args;

                    args.emplace_back(DeclType(&op.Left.value()));

                    if (op.Right->Operator)
                    {
                        auto otherArgs = std::move(getArgs(*op.Right->Operator.value()));

                        args.insert(args.end(), otherArgs.begin(), otherArgs.end());
                    }
                    else if (op.Right)
                    {
                        args.emplace_back(DeclType(&op.Right.value()));
                    }

                    return std::move(args);
                };

                std::vector<DataType> args;

                if (op->Right)
                {
                    if (op->Right->Operator)
                    {
                        args = std::move(getArgs(*op->Right->Operator.value()));
                    }
                    else if (op->Right->Literal || op->Right->Variable)
                    {
                        args.emplace_back(op->Right->Variable->Type);
                    }
                }

                std::string signature(functionName);

                signature += "(";

                for (auto iter = args.begin(); iter != args.end(); ++iter)
                {
                    signature += iter->Str();

                    if (iter != args.end() - 1)
                    {
                        signature += ", ";
                    }
                }

                signature += ")";

                if (m_FunctionData.find(signature) == m_FunctionData.end())
                {
                    bool corrected = false;
                    std::stringstream error;

                    Logger::SetOStream(error);

                    for (auto & function : m_FunctionData)
                    {
                        if (functionName == function.first.substr(0, function.first.find('(')))
                        {
                            if (args.size() > function.second->Params.size())
                            {
                                FLCN_REL_ERROR("SemanticAnalyzer", "{}: Too many arguments for function '{}', expected {} got {}.\n", op->LineNumber, function.first, function.second->Params.size(), args.size());
                                m_Failed = true;
                                break;
                            }

                            if (args.size() < function.second->Params.size())
                            {
                                FLCN_REL_ERROR("SemanticAnalyzer", "{}: Too few arguments for function '{}', expected {} got {}.\n", op->LineNumber, function.first, function.second->Params.size(), args.size());
                                m_Failed = true;
                                break;
                            }

                            for (size_t i = 0; i < args.size(); i++)
                            {
                                if (IsConvertible(args[i], function.second->Params[i].first))
                                {
                                    args[i] = function.second->Params[i].first;
                                }
                                else
                                {
                                    FLCN_REL_ERROR("SemanticAnalyzer", "{}: No known conversion from '{}' to '{}'.", op->LineNumber, args[i], function.second->Params[i].first);
                                    FLCN_REL_INFO("SemanticAnalyzer", "Argument {} for function '{}'.\n", i, function.second->ReturnType.Str() + " " + function.first);
                                    m_Failed = true;
                                }
                            }

                            signature.clear();

                            signature += functionName;

                            signature += "(";

                            for (auto iter = args.begin(); iter != args.end(); ++iter)
                            {
                                signature += iter->Str();

                                if (iter != args.end() - 1)
                                {
                                    signature += ", ";
                                }
                            }

                            signature += ")";

                            if (m_FunctionData.find(signature) != m_FunctionData.end())
                            {
                                corrected = true;
                                break;
                            }
                        }
                    }
                    
                    Logger::SetOStream(std::cout);

                    if (!corrected)
                    {
                        FLCN_REL_ERROR("SemanticAnalyzer", "{}: No matching function for call to '{}'.\n", op->LineNumber, signature);
                        std::cout<<error.str();
                        m_Failed = true;
                    }
                }
            }

            if (op->Left)
            {
                if (op->Left->Variable)
                {
                    AnalyzeVariable(&op->Left->Variable.value());
                }
                else if (op->Left->Operator)
                {
                    AnalyzeOperator(op->Left->Operator.value());
                }
            }

            if (op->Right)
            {
                if (op->Right->Variable)
                {
                    AnalyzeVariable(&op->Right->Variable.value());
                }
                else if (op->Right->Operator)
                {
                    AnalyzeOperator(op->Right->Operator.value());
                }
            }
        }

        void SemanticAnalyzer::AnalyzeStatement(StatementNode * statement)
        {
            if (statement->IfBlock)
            {
                AnalyzeOperator(&statement->IfBlock->Condition);

                for (auto & statement : statement->IfBlock->Statements)
                {
                    AnalyzeStatement(&statement);
                }
            }
            else if (statement->ElseBlock)
            {
                for (auto & statement : statement->IfBlock->Statements)
                {
                    AnalyzeStatement(&statement);
                }
            }
            else if (statement->WhileBlock)
            {
                AnalyzeOperator(&statement->WhileBlock->Condition);

                for (auto & statement : statement->WhileBlock->Statements)
                {
                    AnalyzeStatement(&statement);
                }
            }
            else if (statement->Operator)
            {
                AnalyzeOperator(&statement->Operator.value());
            }
        }

        void SemanticAnalyzer::AnalyzeFunction(FunctionNode * function)
        {
            m_CurrentFunction = function;

            for (auto & statement : function->Statements)
            {
                AnalyzeStatement(&statement);
            }
        }

        void SemanticAnalyzer::AnalyzeClass(ClassNode * clas)
        {
            m_CurrentClass = clas;

            for (auto & function : clas->PublicMemberFunctions)
            {
                AnalyzeFunction(&function);
            }

            for (auto & function : clas->ProtectedMemberFunctions)
            {
                AnalyzeFunction(&function);
            }

            for (auto & function : clas->PrivateMemberFunctions)
            {
                AnalyzeFunction(&function);
            }

            m_CurrentClass = nullptr;
        }

        void SemanticAnalyzer::AnalyzeModule(ModuleNode * module)
        {
            for (auto & function : module->Functions)
            {
                AnalyzeFunction(&function);
            }

            for (auto & clas : module->Classes)
            {
                AnalyzeClass(&clas);
            }
        }

        bool SemanticAnalyzer::Analyze()
        {
            if (auto module = dynamic_cast<ModuleNode *>(m_AST))
            {
                AnalyzeModule(module);
            }
            
            return !m_Failed;
        }
    }
}
