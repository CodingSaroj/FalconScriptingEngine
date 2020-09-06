#ifndef FALCON_ASM_COMBINER_HPP
#define FALCON_ASM_COMBINER_HPP

#include <vector>

#include "AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class Combiner
        {
        public:
            Combiner(std::vector<ASTNode *> asts);

            ASTNode * combine();

        private:
            std::vector<ASTNode *> m_ASTs;

            ASTNode * combine(ASTNode * ast1, ASTNode * ast2);
        };
    }
}

#endif
