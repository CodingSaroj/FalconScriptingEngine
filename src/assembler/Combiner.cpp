/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "../../pch/FalconPCH.hpp"

#include "Combiner.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Combiner::Combiner(std::vector<ASTNode *> asts)
            : m_ASTs(asts)
        {
        }

        ASTNode * Combiner::Combine(ASTNode * ast1, ASTNode * ast2)
        {
            if (auto module1 = dynamic_cast<ModuleNode *>(ast1))
            {
                if (auto module2 = dynamic_cast<ModuleNode *>(ast2))
                {
                    module1->CodeSection.Routines.insert(
                        module1->CodeSection.Routines.begin(),
                        module2->CodeSection.Routines.begin(),
                        module2->CodeSection.Routines.end()
                    );

                    uint64_t module2CodeSectionSize = 0;

                    for (auto & routine : module2->CodeSection.Routines)
                    {
                        for (auto & label : routine.Labels)
                        {
                            module2CodeSectionSize += label.Instructions.size();
                        }

                        module2CodeSectionSize += 2;
                    }

                    for (auto & routine : module1->DebugSection.Routines)
                    {
                        for (auto & lineMap : routine.LineMaps)
                        {
                            lineMap.StartLocation += module2CodeSectionSize;
                        }
                    }

                    module1->DebugSection.Routines.insert(
                        module1->DebugSection.Routines.begin(),
                        module2->DebugSection.Routines.begin(),
                        module2->DebugSection.Routines.end()
                    );

                    module1->ReflectionSection.Attributes.insert(
                        module1->ReflectionSection.Attributes.begin(),
                        module2->ReflectionSection.Attributes.begin(),
                        module2->ReflectionSection.Attributes.end()
                    );
 
                    module1->ReflectionSection.Functions.insert(
                        module1->ReflectionSection.Functions.begin(),
                        module2->ReflectionSection.Functions.begin(),
                        module2->ReflectionSection.Functions.end()
                    );
                    
                    module1->ReflectionSection.Structures.insert(
                        module1->ReflectionSection.Structures.begin(),
                        module2->ReflectionSection.Structures.begin(),
                        module2->ReflectionSection.Structures.end()
                    );

                    module1->ReflectionSection.Aliases.insert(
                        module1->ReflectionSection.Aliases.begin(),
                        module2->ReflectionSection.Aliases.begin(),
                        module2->ReflectionSection.Aliases.end()
                    );
                }
            }

            return ast1;
        }

        ASTNode * Combiner::Combine()
        {
            if (m_ASTs.size() == 0)
            {
                return nullptr;
            }
            else if (m_ASTs.size() == 1)
            {
                return m_ASTs[0];
            }
            else
            {
                ASTNode * current = m_ASTs[0];

                for (uint64_t i = 1; i < m_ASTs.size(); i++)
                {
                    current = Combine(current, m_ASTs[i]);
                }

                return current;
            }
        }
    }
}
