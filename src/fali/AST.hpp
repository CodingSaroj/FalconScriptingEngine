#ifndef FALCON_FALI_AST_HPP
#define FALCON_FALI_AST_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <cstdint>

namespace Falcon
{
    namespace FALI
    {
        struct ASTNode
        {
            virtual ~ASTNode() = default;
        };

        struct LiteralNode : public ASTNode
        {
            enum LiteralType
            {
                BOOL,
                UINT8,
                UINT16,
                UINT32,
                UINT64,
                INT8,
                INT16,
                INT32,
                INT64,
                FLOAT32,
                FLOAT64
            } Type;

            union
            {
                bool Bool;
                uint8_t U8;
                uint16_t U16;
                uint32_t U32;
                uint64_t U64;
                int8_t I8;
                int16_t I16;
                int32_t I32;
                int64_t I64;
                float F32;
                double F64;
            };

            LiteralNode(bool b);
            LiteralNode(uint8_t u8);
            LiteralNode(uint16_t u16);
            LiteralNode(uint32_t u32);
            LiteralNode(uint64_t u64);
            LiteralNode(int8_t i8);
            LiteralNode(int16_t i16);
            LiteralNode(int32_t i32);
            LiteralNode(int64_t i64);
            LiteralNode(float f32);
            LiteralNode(double f64);
        };

        struct TypeNode : public ASTNode
        {
            std::string TypeName;

            TypeNode(const std::string & typeName);
        };

        struct VariableNode : public ASTNode
        {
            std::string Name;
            TypeNode    Type;

            VariableNode(const std::string & name, const std::string & type);
        };

        struct OperatorNode : public ASTNode
        {
            enum OpType
            {
                // Binary Operators
                AR_ADD,
                AR_SUBTRACT,
                AR_MULTIPLY,
                AR_DIVIDE,
                AR_MODULUS,
                BIT_LEFT_SHIFT,
                BIT_RIGHT_SHIFT,
                BIT_AND,
                BIT_OR,
                BIT_XOR,
                CMP_GREATER,
                CMP_GREATER_OR_EQUAL,
                CMP_LESS,
                CMP_LESS_OR_EQUAL,
                CMP_IS_EQUAL,
                CMP_NOT_EQUAL,
                LOGICAL_AND,
                LOGICAL_OR,
                DECLARE,
                ASSIGNMENT,
                
                // Unary Operators
                BIT_COMPLEMENT,
                LOGICAL_NOT,
            } Type;
            
            ASTNode * Left;
            ASTNode * Right;

            OperatorNode(OpType type, ASTNode * Left, ASTNode * Right);

            constexpr bool IsBinary() { return Left && Right; }
            constexpr bool IsUnary() { return Left && !Right; } 
        };

        struct FunctionNode : public ASTNode
        {
            TypeNode    ReturnType;
            std::string Name;
            std::vector<TypeNode> ParameterTypes;
            
            std::unordered_map<std::string, VariableNode> LocalVariables;
            std::vector<OperatorNode> Statements;

            FunctionNode(const std::string & retType, const std::string & name, std::vector<TypeNode> ParameterTypes);
        };

        struct ModuleNode : public ASTNode
        {
            std::vector<FunctionNode> Functions;
        };
    }
}

#endif
