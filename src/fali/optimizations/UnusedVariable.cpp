#include "Optimizations.hpp"

namespace Falcon
{
    namespace FALI
    {
        namespace Optimizations
        {
            IRModuleNode * UnusedVariable(IRModuleNode * ir)
            {
                for (auto & function : ir->Functions)
                {
                    std::unordered_map<std::string, std::vector<std::vector<IRStatementNode>::iterator>> iterators;

                    for (auto iter = function.Statements.begin(); iter != function.Statements.end(); iter++)
                    {
                        for (auto & var : iter->Changes)
                        {
                            if (iter->Operator.Type == OperatorNode::DECLARE || var.second == IRVarChange::ASSIGNED)
                            {
                                iterators[var.first].emplace_back(iter);
                            }
                            else if (var.second == IRVarChange::USED)
                            {
                                iterators[var.first].clear();
                            }
                        }
                    }

                    for (auto & var : iterators)
                    {
                        for (auto & iter : var.second)
                        {
                            function.Statements.erase(iter);
                        }
                    }
                }

                return ir;
            }
        }
    }
}
