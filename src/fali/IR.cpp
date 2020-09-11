#include "IR.hpp"

namespace Falcon
{
    namespace FALI
    {
        static std::vector<IRStatementNode> OperatorToIRStatements(OperatorNode * op)
        {
            std::vector<IRStatementNode> statements;
            
            if (op->IsUnary())
            {
                if (auto var = dynamic_cast<VariableNode *>(op->Left))
                {
                    statements.emplace_back(*op);
                }
                else if (auto op1 = dynamic_cast<OperatorNode *>(op->Left))
                {
                    auto stmts = OperatorToIRStatements(op1);

                    statements.insert(statements.end(), stmts.begin(), stmts.end());

                    op->Left = (ASTNode *)new VariableNode("", "");

                    statements.emplace_back(*op);
                }
            }
            else if (op->IsBinary())
            {
                if (op->Type == OperatorNode::ASSIGNMENT)
                {
                    if (auto op1 = dynamic_cast<OperatorNode *>(op->Right))
                    {
                        auto stmts = OperatorToIRStatements(op1);

                        statements.insert(statements.end(), stmts.begin(), stmts.end());

                        op->Right = (ASTNode *)new VariableNode("", "");
                    }

                    statements.emplace_back(*op);
                }
                else
                {
                    if (auto literal = dynamic_cast<LiteralNode *>(op->Right))
                    {
                        statements.emplace_back(*op);
                    }
                    else if (auto var = dynamic_cast<VariableNode *>(op->Right))
                    {
                        statements.emplace_back(*op);
                    }
                    else if (auto op1 = dynamic_cast<OperatorNode *>(op->Right))
                    {
                        auto stmts = OperatorToIRStatements(op1);

                        statements.insert(statements.end(), stmts.begin(), stmts.end());

                        op->Right = (ASTNode *)new VariableNode("", "");

                        statements.emplace_back(*op);
                    }
                }
            }

            return std::move(statements);
        }

        IRStatementNode::IRStatementNode(OperatorNode op)
            : Operator(op)
        {
        }

        IRModuleNode * ASTToIR(ASTNode * ast)
        {
            IRModuleNode * module = new IRModuleNode();

            if (auto astModule = dynamic_cast<ModuleNode *>(ast))
            {
                for (auto & function : astModule->Functions)
                {
                    IRFunctionNode irFunc;

                    for (auto op : function.Statements)
                    {
                        auto statements = OperatorToIRStatements(&op);

                        irFunc.Statements.insert(irFunc.Statements.begin(), statements.begin(), statements.end());
                    }

                    for (auto & statement : irFunc.Statements)
                    {
                        for (auto var : function.LocalVariables)
                        {
                            IRVarChange change = IRVarChange::NONE;
                            
                            if (auto v = dynamic_cast<VariableNode *>(statement.Operator.Left))
                            {
                                if (statement.Operator.Type == OperatorNode::ASSIGNMENT && v->Name == var.first)
                                {
                                    change = IRVarChange::ASSIGNED;
                                }
                                else if (auto v1 = dynamic_cast<VariableNode *>(statement.Operator.Right))
                                {
                                    if (v->Name == var.first && v1->Name == var.first)
                                    {
                                        change = IRVarChange::USED;
                                    }
                                }
                            }

                            if (auto v = dynamic_cast<VariableNode *>(statement.Operator.Right))
                            {
                                if (statement.Operator.Type == OperatorNode::DECLARE && v->Name == var.first)
                                {
                                    change = IRVarChange::DECLARED;
                                }
                            }

                            statement.Changes[var.first] = change;
                        }
                    }

                    module->Functions.emplace_back(irFunc);
                }
            }

            return module;
        }
    }
}
