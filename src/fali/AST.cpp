/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "AST.hpp"

namespace Falcon
{
    namespace FALI
    {
        LiteralNode::LiteralNode(bool b, uint64_t line)
            : Boolean(b)
        {
            LineNumber = line;
        }

        LiteralNode::LiteralNode(uint64_t whole, uint64_t line)
            : Whole(whole)
        {
            LineNumber = line;
        }

        LiteralNode::LiteralNode(int64_t integer, uint64_t line)
            : Integer(integer)
        {
            LineNumber = line;
        }

        LiteralNode::LiteralNode(double real, uint64_t line)
            : Real(real)
        {
            LineNumber = line;
        }

        VariableNode::VariableNode(const std::string & name, const DataType & type, uint64_t line)
            : Name(name), Type(type)
        {
            LineNumber = line;
        }

        VariableNode::VariableNode(const std::string & name, const std::string & type, const TypeFlag & flags, uint64_t line)
            : Name(name), Type{type, flags}
        {
            LineNumber = line;
        }

        ValueNode::ValueNode(LiteralNode literal, uint64_t line)
            : Literal(literal)
        {
            LineNumber = line;
        }

        ValueNode::ValueNode(VariableNode variable, uint64_t line)
            : Variable(variable)
        {
            LineNumber = line;
        }

        ValueNode::ValueNode(OperatorNode * op, uint64_t line)
            : Operator(op)
        {
            LineNumber = line;
        }

        OperatorNode::OperatorNode(char type, uint64_t line)
            : Binary(false), Type(type)
        {
            LineNumber = line;
        }

        OperatorNode::OperatorNode(char type, ValueNode left, uint64_t line)
            : Binary(false), Type(type), Left(left)
        {
            LineNumber = line;
        }
       
        OperatorNode::OperatorNode(char type, ValueNode left, ValueNode right, uint64_t line)
            : Binary(true), Type(type), Left(left), Right(right)
        {
            LineNumber = line;
        }

        IfNode::IfNode(OperatorNode condition, uint64_t line)
            : Condition(condition)
        {
            LineNumber = line;
        }

        WhileNode::WhileNode(OperatorNode condition, uint64_t line)
            : Condition(condition)
        {
            LineNumber = line;
        }

        StatementNode::StatementNode(OperatorNode operatorNode, uint64_t line)
            : Operator(operatorNode)
        {
            LineNumber = line;
        }

        StatementNode::StatementNode(IfNode ifBlock, uint64_t line)
            : IfBlock(ifBlock)
        {
            LineNumber = line;
        }

        StatementNode::StatementNode(ElseNode elseBlock, uint64_t line)
            : ElseBlock(elseBlock)
        {
            LineNumber = line;
        }

        StatementNode::StatementNode(WhileNode whileBlock, uint64_t line)
            : WhileBlock(whileBlock)
        {
            LineNumber = line;
        }

        FunctionNode::FunctionNode(const DataType & retType, const std::string & name, const std::vector<std::pair<DataType, std::string>> & params, uint64_t line)
            : ReturnType(retType), Name(name), Params(params)
        {
            LineNumber = line;

            for (auto & param : Params)
            {
                LocalVariables[param.second] = FALI::VariableNode(param.second, param.first.Name, param.first.Flags);
            }
        }

        FunctionNode::FunctionNode(const std::string & retType, const std::string & name, const std::vector<std::pair<DataType, std::string>> & params, const TypeFlag & retTypeFlags, uint64_t line)
            : ReturnType{retType, retTypeFlags}, Name(name), Params(params)
        {
            LineNumber = line;

            for (auto & param : Params)
            {
                LocalVariables[param.second] = FALI::VariableNode(param.second, param.first.Name, param.first.Flags);
            }
        }

        std::string FunctionNode::GetSignature()
        {
            std::string signature(Name);

            signature += '(';

            for (auto iter = Params.begin(); iter != Params.end(); ++iter)
            {
                signature += iter->first.Str();

                if (iter != Params.end() - 1)
                {
                    signature += ", ";
                }
            }

            signature += ')';

            return std::move(signature);
        }

        ClassNode::ClassNode(const std::string & name, uint64_t line)
            : Name(name)
        {
            LineNumber = line;
        }
    }
}
