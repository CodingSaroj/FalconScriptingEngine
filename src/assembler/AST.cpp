#include "AST.hpp"

namespace Falcon
{
    namespace Assembler
    {
        AtomNode::AtomNode(std::string reg)
            : Type(AtomType::REGISTER), Register(reg)
        {
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

        InstructionNode::InstructionNode(std::string inst)
            : Inst(inst)
        {
        }

        RoutineNode::RoutineNode(std::string name)
            : Name(name)
        {
        }
    }
}
