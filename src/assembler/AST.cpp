#include "AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        AtomNode::AtomNode(const std::string & str)
            : Str(str)
        {
            if (std::find(RegisterType::s_Names.begin(), RegisterType::s_Names.end(), str) != RegisterType::s_Names.end())
            {
                Type = AtomType::REGISTER;
            }
            else if (str[0] == '@')
            {
                if (std::find(RegisterType::s_Names.begin(), RegisterType::s_Names.end(), str.substr(1)) != RegisterType::s_Names.end())
                {
                    Type = AtomType::REGISTER;
                }
            }
            else if (str[0] == '[' && str[str.size() - 1] == ']')
            {
                if (std::find(RegisterType::s_Names.begin(), RegisterType::s_Names.end(), str.substr(1, str.size() - 2)) != RegisterType::s_Names.end())
                {
                    Type = AtomType::REGISTER;
                }
            }
            else
            {
                Type = AtomType::IDENTIFIER;
            }
        }

        AtomNode::AtomNode(char value)
            : Type(AtomType::CHAR), Char(value)
        {
        }

        AtomNode::AtomNode(uint64_t value)
            : Type(AtomType::UINT), Uint(value)
        {
        }

        AtomNode::AtomNode(int64_t value)
            : Type(AtomType::INT), Int(value)
        {
        }

        AtomNode::AtomNode(double value)
            : Type(AtomType::FLOAT), Float(value)
        {
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
