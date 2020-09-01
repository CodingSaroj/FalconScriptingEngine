#ifndef FALCON_ASM_AST_HPP
#define FALCON_ASM_AST_HPP

#include <algorithm>
#include <string>
#include <vector>

#include <cstdint>

#include "../vm/Register.hpp"

namespace Falcon
{
    namespace Assembler
    {
        struct ASTNode
        {
            uint64_t Line;
            uint64_t Character;

            virtual ~ASTNode() = default;
        };

        struct AtomNode : public ASTNode
        {
            enum struct AtomType
            {
                IDENTIFIER,
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

            std::string Str;

            AtomNode(const std::string & str);
            AtomNode(char data);
            AtomNode(uint64_t data);
            AtomNode(int64_t data);
            AtomNode(double data);
        };

        struct InstructionNode : public ASTNode
        {
            std::string             Inst;
            std::vector<AtomNode>   Args;

            InstructionNode(const std::string & inst);
        };

        struct LabelNode : public ASTNode
        {
            std::string                  Name;
            std::vector<InstructionNode> Instructions;

            LabelNode(const std::string & name);
        };

        struct RoutineNode : public ASTNode
        {
            std::string            Name;
            std::vector<LabelNode> Labels;

            RoutineNode(const std::string & name);
        };

        struct CodeSectionNode : public ASTNode
        {
            std::vector<RoutineNode> Routines;
        };

        struct DebugMetaNode : public ASTNode
        {
            std::string Signature;

            DebugMetaNode(const std::string & signature);
        };

        struct DebugLineMapNode : public ASTNode
        {
            uint64_t StartLocation;
            uint64_t LineNumber;
            std::string LineData;

            DebugLineMapNode(uint64_t startLoc, uint64_t lineNum, const std::string & lineData);
        };

        struct DebugLocalVarNode : public ASTNode
        {
            std::string Name;
            std::string Type;
            uint64_t    StackOffset;

            DebugLocalVarNode(const std::string & name, const std::string & type, uint64_t stackOffset);
        };

        struct DebugRoutineNode : public ASTNode
        {
            std::string                    Name;
            DebugMetaNode                  MetaData;
            std::vector<DebugLineMapNode>  LineMaps;
            std::vector<DebugLocalVarNode> LocalVariables;

            DebugRoutineNode(const std::string & name, const std::string & signature);
        };

        struct DebugSectionNode : public ASTNode
        {
            std::vector<DebugRoutineNode> Routines;
        };

        struct ModuleNode : public ASTNode
        {
            CodeSectionNode CodeSection;
            DebugSectionNode DebugSection;
        };
    }
}

#endif
