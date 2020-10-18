/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "FalconPCH.hpp"

#include "AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        AtomNode::AtomNode(char value)
            : Char(value)
        {
        }

        AtomNode::AtomNode(uint64_t value)
            : Uint(value)
        {
        }

        AtomNode::AtomNode(int64_t value)
            : Int(value)
        {
        }

        AtomNode::AtomNode(double value)
            : Float(value)
        {
        }

        AtomNode::AtomNode(const std::string & str)
        {
            if (std::find(RegisterType::s_Names.begin(), RegisterType::s_Names.end(), str) != RegisterType::s_Names.end())
            {
                Register = Token::RegisterType{str};
            }
            else if (str[0] == '@')
            {
                if (std::find(RegisterType::s_Names.begin(), RegisterType::s_Names.end(), str.substr(1)) != RegisterType::s_Names.end())
                {
                    Register = Token::RegisterType{str};
                }
            }
            else if (str[0] == '[' && str[str.size() - 1] == ']')
            {
                if (std::find(RegisterType::s_Names.begin(), RegisterType::s_Names.end(), str.substr(1, str.size() - 2)) != RegisterType::s_Names.end())
                {
                    Register = Token::RegisterType{str};
                }
            }
            else
            {
                Str = str;
            }
        }

        AtomNode::UnionType AtomNode::GetUnion()
        {
            UnionType unionData;

            if (Char)
            {
                unionData.Char = Char.value();
            }
            else if (Uint)
            {
                unionData.Uint = Uint.value();
            }
            else if (Int)
            {
                unionData.Int = Int.value();
            }
            else if (Float)
            {
                unionData.Float = Float.value();
            }

            return unionData;
        }

        InstructionNode::InstructionNode(const std::string & inst)
            : Inst(inst)
        {
        }

        LabelNode::LabelNode(const std::string & name)
            : Name(name)
        {
        }

        RoutineNode::RoutineNode(const std::string & name)
            : Name(name)
        {
        }

        DebugMetaNode::DebugMetaNode(const std::string & signature, uint64_t startLine, uint64_t endLine)
            : Signature(signature), StartLine(startLine), EndLine(endLine)
        {
        }

        DebugLineMapNode::DebugLineMapNode(uint64_t startLoc, uint64_t lineNum, const std::string & lineData)
            : StartLocation(startLoc), LineNumber(lineNum), LineData(lineData)
        {
        }

        DebugLocalVarNode::DebugLocalVarNode(const std::string & name, const std::string & type, uint64_t stackOffset)
            : Name(name), Type(type), StackOffset(stackOffset)
        {
        }

        DebugRoutineNode::DebugRoutineNode(const std::string & name, const std::string & signature, uint64_t startLine, uint64_t endLine)
            : Name(name), MetaData(signature, startLine, endLine)
        {
        }


        ReflectionAttributeNode::ReflectionAttributeNode(const std::string & name, std::vector<std::string> attribs)
            : Name(name), Attributes(attribs)
        {
        }

        ReflectionFunctionNode::ReflectionFunctionNode(const std::string & name, const std::string & retType, std::vector<std::string> params)
            : Name(name), ReturnType(retType), Parameters(params) 
        {
        }

        ReflectionStructureNode::ReflectionStructureNode(const std::string & name, std::vector<Member> members)
            : Name(name), Members(members)
        {
        }

        ReflectionAliasNode::ReflectionAliasNode(const std::string & name, const std::string & base)
            : Name(name), Base(base)
        {
        }
    }
}
