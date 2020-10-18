/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_FALI_SEMANTIC_ANALYZER_HPP
#define FALCON_FALI_SEMANTIC_ANALYZER_HPP

#include "common/Common.hpp"
#include "common/Logger.hpp"

#include "fali/AST.hpp"

namespace Falcon
{
    namespace FALI
    {
        class SemanticAnalyzer
        {
        public:
            explicit SemanticAnalyzer(ASTNode * ast);

            bool Analyze();

        private:
            bool m_Failed;

            ASTNode * m_AST;

            FunctionNode * m_CurrentFunction;
            ClassNode *    m_CurrentClass;

            std::unordered_map<std::string, FunctionNode *> m_FunctionData;
            std::unordered_map<std::string, ClassNode *>    m_ClassData;

            void CollectSemanticData();

            DataType DeclType(ValueNode * value);
            bool IsConvertible(const DataType & from, const DataType & to);

            void AnalyzeVariable(VariableNode * var);
            void AnalyzeOperator(OperatorNode * op);
            void AnalyzeStatement(StatementNode * statement);
            void AnalyzeFunction(FunctionNode * function);
            void AnalyzeClass(ClassNode * clas);
            void AnalyzeModule(ModuleNode * module);
        };
    }
}

#endif
