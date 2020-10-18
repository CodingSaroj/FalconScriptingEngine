/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "../../pch/FalconPCH.hpp"

#include "Serialize.hpp"

namespace Falcon
{
    namespace FALI
    {
        void Serialize(ASTNode * ast, std::string padding)
        {
            if (!ast)
            {
                return;
            }
            
            if (auto literal = dynamic_cast<LiteralNode *>(ast))
            {
                std::cout<<Format("{}Literal: ", padding);

                if (literal->Boolean)
                {
                    std::cout<<Format("{}\n", *literal->Boolean ? "true" : "false");
                }
                else if (literal->Whole)
                {
                    std::cout<<Format("{}\n", literal->Whole.value());
                }
                else if (literal->Integer)
                {
                    std::cout<<Format("{}\n", literal->Integer.value());
                }
                else if (literal->Real)
                {
                    std::cout<<Format("{}\n", literal->Real.value());
                }
            }
            else if (auto var = dynamic_cast<VariableNode *>(ast))
            {
                if (var->Name.empty())
                {
                    std::cout<<Format("{}Type: {}\n", padding, var->Type);
                }
                else
                {
                    std::cout<<Format("{}Variable: {} {}\n", padding, var->Type, var->Name);
                }
            }
            else if (auto value = dynamic_cast<ValueNode *>(ast))
            {
                if (value->Literal)
                {
                    Serialize(&value->Literal.value(), padding);
                }
                else if (value->Variable)
                {
                    Serialize(&value->Variable.value(), padding);
                }
                else if (value->Operator)
                {
                    Serialize(value->Operator.value(), padding);
                }
            }
            else if (auto op = dynamic_cast<OperatorNode *>(ast))
            {
                std::cout<<Format("{}Operator {}:\n", padding, op->Type);

                if (op->Left)
                {
                    Serialize(&op->Left.value(), padding + "    ");
                }

                if (op->Right)
                {
                    Serialize(&op->Right.value(), padding + "    ");
                }
            }
            else if (auto statement = dynamic_cast<StatementNode *>(ast))
            {
                if (statement->Operator)
                {
                    std::cout<<Format("{}Statement Operator:\n", padding);

                    Serialize(&statement->Operator.value(), padding + "    ");
                }
                else if (statement->IfBlock)
                {
                    std::cout<<Format("{}Statement if:\n", padding);

                    Serialize(&statement->IfBlock->Condition, padding + "    ");

                    for (auto & statement : statement->IfBlock->Statements)
                    {
                        Serialize(&statement, padding + "    ");
                    }
                }
                else if (statement->ElseBlock)
                {
                    std::cout<<Format("{}Statement else:\n", padding);

                    for (auto & statement : statement->ElseBlock->Statements)
                    {
                        Serialize(&statement, padding + "    ");
                    }
                }
                else if (statement->WhileBlock)
                {
                    std::cout<<Format("{}Statement while:\n", padding);

                    Serialize(&statement->WhileBlock->Condition, padding + "    ");

                    for (auto & statement : statement->WhileBlock->Statements)
                    {
                        Serialize(&statement, padding + "    ");
                    }
                }
            }
            else if (auto function = dynamic_cast<FunctionNode *>(ast))
            {
                std::cout<<Format("{}Function {} {}(", padding, function->ReturnType, function->Name);

                for (auto & param : function->Params)
                {
                    std::cout<<Format("{} {}, ", param.first, param.second);
                }

                if (function->Params.size())
                {
                    std::cout<<"\b\b";
                }

                std::cout<<"):\n";

                for (auto & statement : function->Statements)
                {
                    Serialize(&statement, padding + "    ");
                }
            }
            else if (auto clas = dynamic_cast<ClassNode *>(ast))
            {
                std::cout<<Format("{}Class {}:\n", padding, clas->Name);

                std::cout<<Format("{}Public:\n", padding + "    ");

                for (auto & var : clas->PublicMemberVariables)
                {
                    Serialize(&var, padding + "        ");
                }

                for (auto & function : clas->PublicMemberFunctions)
                {
                    Serialize(&function, padding + "        ");
                }

                std::cout<<Format("{}Protected:\n", padding + "    ");

                for (auto & var : clas->ProtectedMemberVariables)
                {
                    Serialize(&var, padding + "        ");
                }

                for (auto & function : clas->ProtectedMemberFunctions)
                {
                    Serialize(&function, padding + "        ");
                }

                std::cout<<Format("{}Private:\n", padding + "    ");

                for (auto & var : clas->PrivateMemberVariables)
                {
                    Serialize(&var, padding + "        ");
                }

                for (auto & function : clas->PrivateMemberFunctions)
                {
                    Serialize(&function, padding + "        ");
                }
            }
            else if (auto module = dynamic_cast<ModuleNode *>(ast))
            {
                std::cout<<Format("{}Module:\n", padding);

                for (auto & clas : module->Classes)
                {
                    Serialize(&clas, padding + "    ");
                }

                for (auto & function : module->Functions)
                {
                    Serialize(&function, padding + "    ");
                }
            }
        }
/*
        void Serialize(IRFunction & function)
        {
            std::cout<<function.ReturnType<<" "<<function.Name<<"(";

            for (auto & params : function.Params)
            {
                std::cout<<params.first<<" "<<params.second<<", ";
            }

            if (function.Params.size())
            {
                std::cout<<"\b\b";
            }

            std::cout<<"):\n";

            size_t statementCounter = 0;

            for (auto & statement : function.Statements)
            {
                for (auto iter = function.Labels.begin(); iter != function.Labels.end(); ++iter)
                {
                    if (iter->second.first == statementCounter)
                    {
                        if (iter != function.Labels.begin())
                        {
                            std::cout<<"\n";
                        }

                        std::cout<<"."<<iter->first<<":\n";

                        if (iter->second.first != iter->second.second)
                        {
                            break;
                        }
                    }
                }

                if (statement.Operation == '@')
                {
                    std::cout<<"    goto "<<statement.V2<<"\n";
                }
                else if (statement.Operation == 'i')
                {
                    std::cout<<"    if "<<statement.V2<<"\n";
                }
                else if (statement.Operation == 'e')
                {
                    std::cout<<"    else "<<statement.V2<<"\n";
                }
                else if (statement.Operation == '=')
                {
                    std::cout<<"    "<<statement.V1<<" := "<<statement.V2<<"\n";
                }
                else if (statement.Operation == 'R')
                {
                    std::cout<<"    ret "<<statement.V1<<"\n";
                }
                else if (statement.V3 == "")
                {
                    std::cout<<"    "<<statement.V1<<" := "<<statement.Operation<<" "<<statement.V2<<"\n";
                }
                else
                {
                    std::cout<<"    "<<statement.V1<<" := "<<statement.V2<<" "<<statement.Operation<<" "<<statement.V3<<"\n";
                }

                ++statementCounter;
            }
        }*/
    }
}
