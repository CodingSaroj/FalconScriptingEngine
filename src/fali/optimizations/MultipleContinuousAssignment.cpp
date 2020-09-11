#include "Optimizations.hpp"

namespace Falcon
{
    namespace FALI
    {
        namespace Optimizations
        {
            IRModuleNode * MultipleContinuousAssignment(IRModuleNode * module)
            {
                for (auto & function : module->Functions)
                {
                    std::vector<std::vector<IRStatementNode>::iterator> iteratorsToRemove;
                    std::unordered_map<std::string, std::pair<std::vector<IRStatementNode>::iterator, IRVarChange>> changes;

                    for (auto iter = function.Statements.begin(); iter != function.Statements.end(); iter++)
                    {
                        for (auto change : iter->Changes)
                        {
                            if (change.second != IRVarChange::NONE)
                            {
                                if (change.second == IRVarChange::ASSIGNED && changes[change.first].second == IRVarChange::ASSIGNED)
                                {
                                    iteratorsToRemove.emplace_back(changes[change.first].first);
                                }
                                else
                                {
                                    changes[change.first] = {iter, change.second};
                                }
                            }
                        }
                    }

                    for (auto iter : iteratorsToRemove)
                    {
                        function.Statements.erase(iter);
                    }
                }

                return module;
            }
        }
    }
}
