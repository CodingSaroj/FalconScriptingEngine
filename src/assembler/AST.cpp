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
    }
}
