#include "AST.hpp"

namespace Falcon
{
    namespace FALI
    {
        LiteralNode::LiteralNode(bool b)
            : Type(BOOL), Bool(b)
        {
        }


        LiteralNode::LiteralNode(uint8_t u8)
            : Type(UINT8), U8(u8)
        {
        }

        LiteralNode::LiteralNode(uint16_t u16)
            : Type(UINT16), U16(u16)
        {
        }

        LiteralNode::LiteralNode(uint32_t u32)
            : Type(UINT32), U32(u32)
        {
        }

        LiteralNode::LiteralNode(uint64_t u64)
            : Type(UINT64), U64(u64)
        {
        }

        LiteralNode::LiteralNode(int8_t i8)
            : Type(INT8), I8(i8)
        {
        }

        LiteralNode::LiteralNode(int16_t i16)
            : Type(INT16), I16(i16)
        {
        }

        LiteralNode::LiteralNode(int32_t i32)
            : Type(INT32), I32(i32)
        {
        }

        LiteralNode::LiteralNode(int64_t i64)
            : Type(INT64), I64(i64)
        {
        }

        LiteralNode::LiteralNode(float f32)
            : Type(FLOAT32), F32(f32)
        {
        }
        
        LiteralNode::LiteralNode(double f64)
            : Type(FLOAT64), F64(f64)
        {
        }
        
        TypeNode::TypeNode(const std::string & typeName)
            : TypeName(typeName)
        {
        }

        VariableNode::VariableNode(const std::string & name, const std::string & type)
            : Name(name), Type(type)
        {
        }
        
        OperatorNode::OperatorNode(OpType type, ASTNode * left, ASTNode * right)
            : Type(type), Left(left), Right(right)
        {
        }

        FunctionNode::FunctionNode(const std::string & retType, const std::string & name, std::vector<TypeNode> paramTypes)
            : ReturnType(retType), Name(name), ParameterTypes(paramTypes)
        {
        }
    }
}
