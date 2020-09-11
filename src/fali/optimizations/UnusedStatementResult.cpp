#include "Optimizations.hpp"

namespace Falcon
{
    namespace FALI
    {
        namespace Optimizations
        {
            IRModuleNode * UnusedStatementResult(IRModuleNode * module)
            {
                for (auto & function : module->Functions)
                {
                    for (auto iter = function.Statements.begin(); iter != function.Statements.end(); iter++)
                    {
                        auto & statement = *iter;

                        auto nextIter = iter + 1;

                        if (nextIter == function.Statements.end())
                        {
                            break;
                        }

                        if (statement.Operator.Type == OperatorNode::ASSIGNMENT || statement.Operator.Type == OperatorNode::DECLARE)
                        {
                            continue;
                        }

                        auto & nextStatement = *nextIter;

                        auto left = dynamic_cast<VariableNode *>(nextStatement.Operator.Left);
                        auto right = dynamic_cast<VariableNode *>(nextStatement.Operator.Right);

                        auto prevIter = iter - 1;

                        if (left && right)
                        {
                            if (left->Name != "" && right->Name != "")
                            {
                                function.Statements.erase(iter);

                                iter = prevIter;
                            }
                        }
                        else if (left && left->Name != "")
                        {
                            function.Statements.erase(iter);

                            iter = prevIter;
                        }
                        else if (right && right->Name != "")
                        {
                            function.Statements.erase(iter);

                            iter = prevIter;
                        }
                        else
                        {
                            function.Statements.erase(iter);

                            iter = prevIter;
                        }
                    }
                }

                return module;
            }
        }
    }
}
