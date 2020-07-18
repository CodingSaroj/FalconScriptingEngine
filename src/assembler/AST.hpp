#ifndef FALCON_ASM_AST_HPP
#define FALCON_ASM_AST_HPP

#include <string>
#include <vector>

#include <cstdint>

namespace Falcon
{
    namespace Assembler
    {
        class ASTNode
        {
            public:
                virtual ~ASTNode() = default;
        };

        class AtomNode : public ASTNode
        {
            public:
                enum class AtomType
                {
                    REGISTER,
                    CHAR,
                    UINT,
                    INT,
                    FLOAT
                } Type;

                union
                {
                    char     Char;
                    uint64_t Uint;
                    int64_t  Int;
                    double   Float;
                };
                std::string Register;

                AtomNode(std::string reg);
                AtomNode(char data);
                AtomNode(uint64_t data);
                AtomNode(int64_t data);
                AtomNode(double data);
        };

        class InstructionNode : public ASTNode
        {
            public:
                std::string             Inst;
                std::vector<AtomNode>   Args;

                InstructionNode(std::string inst);
        };

        class RoutineNode : public ASTNode
        {
            public:
                std::string                  Name;
                std::vector<InstructionNode> Instructions;

                RoutineNode(std::string name);
        };

        class CodeSectionNode : public ASTNode
        {
            public:
                std::vector<RoutineNode> Routines;
        };
    }
}

#endif
