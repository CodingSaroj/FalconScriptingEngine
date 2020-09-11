#include "Serialize.hpp"

namespace Falcon
{
    namespace FALI
    {
        static std::unordered_map<OperatorNode::OpType, std::string> opNames
        {
            {OperatorNode::AR_ADD,               "+"},
            {OperatorNode::AR_SUBTRACT,          "-"},
            {OperatorNode::AR_MULTIPLY,          "*"},
            {OperatorNode::AR_DIVIDE,            "/"},
            {OperatorNode::AR_MODULUS,           "%"},
            {OperatorNode::BIT_LEFT_SHIFT,       "<<"},
            {OperatorNode::BIT_RIGHT_SHIFT,      ">>"},
            {OperatorNode::BIT_AND,              "&"},
            {OperatorNode::BIT_OR,               "|"},
            {OperatorNode::BIT_XOR,              "^"},
            {OperatorNode::CMP_GREATER,          ">"},
            {OperatorNode::CMP_GREATER_OR_EQUAL, ">="},
            {OperatorNode::CMP_LESS,             "<"},
            {OperatorNode::CMP_LESS_OR_EQUAL,    "<="},
            {OperatorNode::CMP_IS_EQUAL,         "=="},
            {OperatorNode::CMP_NOT_EQUAL,        "!="},
            {OperatorNode::LOGICAL_AND,          "&&"},
            {OperatorNode::LOGICAL_OR,           "||"},
            {OperatorNode::DECLARE,              "decl"},
            {OperatorNode::ASSIGNMENT,           "="},
            {OperatorNode::BIT_COMPLEMENT,       "~"},
            {OperatorNode::LOGICAL_NOT,          "!"}
        };

        void Serialize(ASTNode * ast, const std::string & padding)
        {
            if (!ast)
            {
                return;
            }

            if (auto literal = dynamic_cast<LiteralNode *>(ast))
            {
                std::cout<<padding<<"Literal: ";

                switch (literal->Type)
                {
                    case LiteralNode::BOOL:
                    {
                        std::cout<<(literal->Bool ? "true" : "false");
                        break;
                    }


                    case LiteralNode::UINT8:
                    {
                        std::cout<<literal->U8;
                        break;
                    }

                    case LiteralNode::UINT16:
                    {
                        std::cout<<literal->U16;
                        break;
                    }

                    case LiteralNode::UINT32:
                    {
                        std::cout<<literal->U32;
                        break;
                    }

                    case LiteralNode::UINT64:
                    {
                        std::cout<<literal->U64;
                        break;
                    }

                    case LiteralNode::INT8:
                    {
                        std::cout<<literal->I8;
                        break;
                    }

                    case LiteralNode::INT16:
                    {
                        std::cout<<literal->I16;
                        break;
                    }

                    case LiteralNode::INT32:
                    {
                        std::cout<<literal->I32;
                        break;
                    }
                    
                    case LiteralNode::INT64:
                    {
                        std::cout<<literal->I64;
                        break;
                    }

                    case LiteralNode::FLOAT32:
                    {
                        std::cout<<literal->F32;
                        break;
                    }

                    case LiteralNode::FLOAT64:
                    {
                        std::cout<<literal->F64;
                        break;
                    }
                }

                std::cout<<"\n";
            }
            else if (auto type = dynamic_cast<TypeNode *>(ast))
            {
                std::cout<<padding<<"Type: "<<type->TypeName<<"\n";
            }
            else if (auto var = dynamic_cast<VariableNode *>(ast))
            {
                std::cout<<padding<<"Variable: ";

                if (var->Name == "" && var->Type.TypeName == "")
                {
                    std::cout<<"$"<<"\n";
                }
                else
                {
                    std::cout<<var->Type.TypeName<<" "<<var->Name<<"\n";
                } 
            }
            else if (auto op = dynamic_cast<OperatorNode *>(ast))
            {
                std::cout<<padding<<"Operator: "<<opNames[op->Type]<<"\n";

                Serialize(op->Left, padding + "    ");
                Serialize(op->Right, padding + "    ");
            }
            else if (auto function = dynamic_cast<FunctionNode *>(ast))
            {
                std::cout<<padding<<"Function: "<<function->ReturnType.TypeName<<" "<<function->Name<<"(";

                for (auto paramType : function->ParameterTypes)
                {
                    std::cout<<paramType.TypeName<<", ";
                }

                std::cout<<")\n";

                for (auto op : function->Statements)
                {
                    Serialize(&op, padding + "    ");
                }
            }
            else if (auto module = dynamic_cast<ModuleNode *>(ast))
            {
                std::cout<<padding<<"Module:\n";

                for (auto func : module->Functions)
                {
                    Serialize(&func, padding + "    ");
                }
            }
        }

        void Serialize(IRNode * ir, const std::string & padding)
        {
            if (!ir)
            {
                return;
            }

            if (auto statement = dynamic_cast<IRStatementNode *>(ir))
            {
                std::cout<<padding<<"Statement: "<<opNames[statement->Operator.Type]<<"\n";

                for (auto change : statement->Changes)
                {
                    std::cout<<padding<<"    "<<change.first<<" ";

                    switch (change.second)
                    {
                        case IRVarChange::NONE:
                        {
                            std::cout<<"NONE\n";
                            break;
                        }

                        case IRVarChange::DECLARED:
                        {
                            std::cout<<"DECLARED\n";
                            break;
                        }

                        case IRVarChange::USED:
                        {
                            std::cout<<"USED\n";
                            break;
                        }

                        case IRVarChange::ASSIGNED:
                        {
                            std::cout<<"ASSIGNED\n";
                            break;
                        }
                    }
                }

                Serialize(statement->Operator.Left, padding + "    ");
                Serialize(statement->Operator.Right, padding + "    ");
            }
            else if (auto func = dynamic_cast<IRFunctionNode *>(ir))
            {
                std::cout<<padding<<"Function:\n";

                for (auto statement : func->Statements)
                {
                    Serialize(&statement, padding + "    ");
                }
            }
            else if (auto module = dynamic_cast<IRModuleNode *>(ir))
            {
                std::cout<<padding<<"Module:\n";

                for (auto function : module->Functions)
                {
                    Serialize(&function, padding + "    ");
                }
            }
        }
    }
}
