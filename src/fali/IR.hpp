#ifndef FALCON_FALI_IR_HPP
#define FALCON_FALI_IR_HPP

#include "fali/AST.hpp"

namespace Falcon
{
    namespace FALI
    {
        enum class IRVarChange
        {
            NONE,
            DECLARED,
            USED,
            ASSIGNED
        };

        struct IRState
        {
            std::unordered_map<std::string, IRVarChange> Changes;
        };

        struct IRNode
        {
            virtual ~IRNode() = default;
        };
        
        struct IRStatementNode : public IRNode, public IRState
        {
            OperatorNode Operator;

            IRStatementNode(OperatorNode op);
        };

        struct IRFunctionNode : public IRNode
        {
            std::vector<IRStatementNode> Statements;
        };
        
        struct IRModuleNode : public IRNode
        {
            std::vector<IRFunctionNode> Functions;
        };

        IRModuleNode * ASTToIR(ASTNode * ast);
    }
}

#endif
