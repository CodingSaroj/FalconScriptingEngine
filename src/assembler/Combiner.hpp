/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_ASM_COMBINER_HPP
#define FALCON_ASM_COMBINER_HPP

#include "common/Common.hpp"

#include "assembler/AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Combiner
        {
        public:
            Combiner(std::vector<ASTNode *> asts);

            ASTNode * Combine();

        private:
            std::vector<ASTNode *> m_ASTs;

            ASTNode * Combine(ASTNode * ast1, ASTNode * ast2);
        };
    }
}

#endif
