#include "Optimizations.hpp"

namespace Falcon
{
    namespace FALI
    {
        namespace Optimizations
        {
            static bool IsConstant(OperatorNode * op)
            {
                auto left = dynamic_cast<LiteralNode *>(op->Left);
                auto right = dynamic_cast<LiteralNode *>(op->Right);

                return left && right;
            }
            
            static size_t getTypeSize(LiteralNode::LiteralType type)
            {
                size_t size = 0;

                switch (type)
                {
                    case LiteralNode::BOOL:
                    {
                        size = 1;
                        break;
                    }


                    case LiteralNode::UINT8:
                    {
                        size = 1;
                        break;
                    }

                    case LiteralNode::UINT16:
                    {
                        size = 2;
                        break;
                    }

                    case LiteralNode::UINT32:
                    {
                        size = 4;
                        break;
                    }

                    case LiteralNode::UINT64:
                    {
                        size = 8;
                        break;
                    }

                    case LiteralNode::INT8:
                    {
                        size = 1;
                        break;
                    }

                    case LiteralNode::INT16:
                    {
                        size = 2;
                        break;
                    }

                    case LiteralNode::INT32:
                    {
                        size = 4;
                        break;
                    }
                    
                    case LiteralNode::INT64:
                    {
                        size = 8;
                        break;
                    }

                    case LiteralNode::FLOAT32:
                    {
                        size = 4;
                        break;
                    }

                    case LiteralNode::FLOAT64:
                    {
                        size = 8;
                        break;
                    }
                }

                return size;
            }

            static size_t getTypeBase(LiteralNode::LiteralType type)
            {
                if (type == LiteralNode::BOOL)
                {
                    return 0;
                }
                else if (type >= LiteralNode::UINT8 && type <= LiteralNode::UINT64)
                {
                    return 1;
                }
                else if (type >= LiteralNode::INT8 && type <= LiteralNode::INT64)
                {
                    return 2;
                }
                else
                {
                    return 3;
                }
            }

            static LiteralNode::LiteralType getHigherType(LiteralNode::LiteralType t1, LiteralNode::LiteralType t2)
            {
                LiteralNode::LiteralType type;

                if (t1 == t2)
                {
                    return t1;
                }

                size_t typeBase = getTypeBase(t1) > getTypeBase(t2) ? getTypeBase(t1) : getTypeBase(t2);
                size_t typeSize = getTypeSize(t1) > getTypeSize(t2) ? getTypeSize(t1) : getTypeSize(t2);

                if (typeBase == 0)
                {
                    type = LiteralNode::BOOL;
                }
                else if (typeBase == 1)
                {
                    if (typeSize == 1)
                    {
                        type = LiteralNode::UINT8;
                    }
                    else if (typeSize == 2)
                    {
                        type = LiteralNode::UINT16;
                    }
                    else if (typeSize == 4)
                    {
                        type = LiteralNode::UINT32;
                    }
                    else if (typeSize == 8)
                    {
                        type = LiteralNode::UINT64;
                    }
                }
                else if (typeBase == 2)
                {
                    if (typeSize == 1)
                    {
                        type = LiteralNode::INT8;
                    }
                    else if (typeSize == 2)
                    {
                        type = LiteralNode::INT16;
                    }
                    else if (typeSize == 4)
                    {
                        type = LiteralNode::INT32;
                    }
                    else if (typeSize == 8)
                    {
                        type = LiteralNode::INT64;
                    }
                }
                else if (typeBase == 3)
                {
                    if (typeSize == 4)
                    {
                        type = LiteralNode::FLOAT32;
                    }
                    else if (typeSize == 8)
                    {
                        type = LiteralNode::FLOAT64;
                    }
                }

                return type;
            }

            static LiteralNode FoldConstants(OperatorNode * op)
            {
                LiteralNode literal((uint8_t)0);

                auto left = dynamic_cast<LiteralNode *>(op->Left);
                auto right = dynamic_cast<LiteralNode *>(op->Right);

                auto castLiteral = [](LiteralNode & literal, LiteralNode::LiteralType type)
                {
                    if (literal.Type == type)
                    {
                        return;
                    }

                    if ((getTypeBase(literal.Type) != 3 && getTypeBase(type) != 3) && literal.Type == LiteralNode::BOOL)
                    {
                        literal.Type = type;
                    }
                    else if ((getTypeBase(literal.Type) != 3 && getTypeBase(type) != 3) && getTypeBase(literal.Type) == getTypeBase(type))
                    {
                        literal.Type = type;
                    }
                    else
                    {
                        if (getTypeBase(literal.Type) != 3 && type == LiteralNode::FLOAT32)
                        {
                            switch (type)
                            {
                                case LiteralNode::BOOL:
                                {
                                    literal.F32 = literal.Bool;
                                    break;
                                }

                                case LiteralNode::UINT8:
                                {
                                    literal.F32 = literal.U8;
                                    break;
                                }

                                case LiteralNode::UINT16:
                                {
                                    literal.F32 = literal.U16;
                                    break;
                                }

                                case LiteralNode::UINT32:
                                {
                                    literal.F32 = literal.U32;
                                    break;
                                }

                                case LiteralNode::UINT64:
                                {
                                    literal.F32 = literal.U64;
                                    break;
                                }

                                case LiteralNode::INT8:
                                {
                                    literal.F32 = literal.I8;
                                    break;
                                }

                                case LiteralNode::INT16:
                                {
                                    literal.F32 = literal.I16;
                                    break;
                                }

                                case LiteralNode::INT32:
                                {
                                    literal.F32 = literal.I32;
                                    break;
                                }

                                case LiteralNode::INT64:
                                {
                                    literal.F32 = literal.I64;
                                    break;
                                }

                                case LiteralNode::FLOAT64:
                                {
                                    literal.F32 = literal.F32;
                                    break;
                                }
                            }

                            literal.Type = type;
                        }
                        else if (getTypeBase(literal.Type) != 3 && type == LiteralNode::FLOAT64)
                        {
                            switch (type)
                            {
                                case LiteralNode::BOOL:
                                {
                                    literal.F64 = literal.Bool;
                                    break;
                                }

                                case LiteralNode::UINT8:
                                {
                                    literal.F64 = literal.U8;
                                    break;
                                }

                                case LiteralNode::UINT16:
                                {
                                    literal.F64 = literal.U16;
                                    break;
                                }

                                case LiteralNode::UINT32:
                                {
                                    literal.F64 = literal.U32;
                                    break;
                                }

                                case LiteralNode::UINT64:
                                {
                                    literal.F64 = literal.U64;
                                    break;
                                }

                                case LiteralNode::INT8:
                                {
                                    literal.F64 = literal.I8;
                                    break;
                                }

                                case LiteralNode::INT16:
                                {
                                    literal.F64 = literal.I16;
                                    break;
                                }

                                case LiteralNode::INT32:
                                {
                                    literal.F64 = literal.I32;
                                    break;
                                }

                                case LiteralNode::INT64:
                                {
                                    literal.F64 = literal.I64;
                                    break;
                                }

                                case LiteralNode::FLOAT64:
                                {
                                    literal.F64 = literal.F32;
                                    break;
                                }
                            }

                            literal.Type = type;
                        }
                        else if (getTypeBase(type) != 3 && literal.Type == LiteralNode::FLOAT32)
                        {
                            switch (type)
                            {
                                case LiteralNode::BOOL:
                                {
                                    literal.Bool = literal.F32;
                                    break;
                                }

                                case LiteralNode::UINT8:
                                {
                                    literal.U8 = literal.F32;
                                    break;
                                }

                                case LiteralNode::UINT16:
                                {
                                    literal.U16 = literal.F32;
                                    break;
                                }

                                case LiteralNode::UINT32:
                                {
                                    literal.U32 = literal.F32;
                                    break;
                                }

                                case LiteralNode::UINT64:
                                {
                                    literal.U64 = literal.F32;
                                    break;
                                }

                                case LiteralNode::INT8:
                                {
                                    literal.I8 = literal.F32;
                                    break;
                                }

                                case LiteralNode::INT16:
                                {
                                    literal.I16 = literal.F32;
                                    break;
                                }

                                case LiteralNode::INT32:
                                {
                                    literal.I32 = literal.F32;
                                    break;
                                }

                                case LiteralNode::INT64:
                                {
                                    literal.I64 = literal.F32;
                                    break;
                                }

                                case LiteralNode::FLOAT64:
                                {
                                    literal.F64 = literal.F32;
                                    break;
                                }
                            }

                            literal.Type = type;
                        }
                        else if (getTypeBase(type) != 3 && literal.Type == LiteralNode::FLOAT64)
                        {
                            switch (type)
                            {
                                case LiteralNode::BOOL:
                                {
                                    literal.Bool = literal.F64;
                                    break;
                                }

                                case LiteralNode::UINT8:
                                {
                                    literal.U8 = literal.F64;
                                    break;
                                }

                                case LiteralNode::UINT16:
                                {
                                    literal.U16 = literal.F64;
                                    break;
                                }

                                case LiteralNode::UINT32:
                                {
                                    literal.U32 = literal.F64;
                                    break;
                                }

                                case LiteralNode::UINT64:
                                {
                                    literal.U64 = literal.F64;
                                    break;
                                }

                                case LiteralNode::INT8:
                                {
                                    literal.I8 = literal.F64;
                                    break;
                                }

                                case LiteralNode::INT16:
                                {
                                    literal.I16 = literal.F64;
                                    break;
                                }

                                case LiteralNode::INT32:
                                {
                                    literal.I32 = literal.F64;
                                    break;
                                }

                                case LiteralNode::INT64:
                                {
                                    literal.I64 = literal.F64;
                                    break;
                                }

                                case LiteralNode::FLOAT32:
                                {
                                    literal.F32 = literal.F64;
                                    break;
                                }
                            }

                            literal.Type = type;
                        }
                    }
                };

                if (!right)
                {
                    literal.Type = left->Type;
                }
                else
                {
                    literal.Type = getHigherType(left->Type, right->Type);
                }

                castLiteral(*left, literal.Type);

                if (right)
                {
                    castLiteral(*right, literal.Type);
                }

                switch (op->Type)
                {
                    case OperatorNode::LOGICAL_AND:
                    {
                        literal.Type = LiteralNode::BOOL;
                        literal.Bool = left->Bool && right->Bool;
                        return literal;
                    }

                    case OperatorNode::LOGICAL_OR:
                    {
                        literal.Type = LiteralNode::BOOL;
                        literal.Bool = left->Bool || right->Bool;
                        return literal;
                    }

                    case OperatorNode::LOGICAL_NOT:
                    {
                        literal.Type = LiteralNode::BOOL;
                        literal.Bool = !left->Bool;
                        return literal;
                    }
                }
                
                if (literal.Type == LiteralNode::UINT8)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.U8 = left->U8 + right->U8;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.U8 = left->U8 - right->U8;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.U8 = left->U8 * right->U8;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.U8 = left->U8 / right->U8;
                            break;
                        }

                        case OperatorNode::AR_MODULUS:
                        {
                            literal.U8 = left->U8 % right->U8;
                            break;
                        }

                        case OperatorNode::BIT_LEFT_SHIFT:
                        {
                            literal.U8 = left->U8 << right->U8;
                            break;
                        }

                        case OperatorNode::BIT_RIGHT_SHIFT:
                        {
                            literal.U8 = left->U8 >> right->U8;
                            break;
                        }

                        case OperatorNode::BIT_AND:
                        {
                            literal.U8 = left->U8 & right->U8;
                            break;
                        }
 
                        case OperatorNode::BIT_OR:
                        {
                            literal.U8 = left->U8 | right->U8;
                            break;
                        }                   

                        case OperatorNode::BIT_XOR:
                        {
                            literal.U8 = left->U8 ^ right->U8;
                            break;
                        }

                        case OperatorNode::BIT_COMPLEMENT:
                        {
                            literal.U8 = ~left->U8;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U8 > right->U8;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U8 >= right->U8;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U8 < right->U8;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U8 <= right->U8;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U8 == right->U8;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U8 != right->U8;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::UINT16)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.U16 = left->U16 + right->U16;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.U16 = left->U16 - right->U16;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.U16 = left->U16 * right->U16;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.U16 = left->U16 / right->U16;
                            break;
                        }

                        case OperatorNode::AR_MODULUS:
                        {
                            literal.U16 = left->U16 % right->U16;
                            break;
                        }

                        case OperatorNode::BIT_LEFT_SHIFT:
                        {
                            literal.U16 = left->U16 << right->U16;
                            break;
                        }

                        case OperatorNode::BIT_RIGHT_SHIFT:
                        {
                            literal.U16 = left->U16 >> right->U16;
                            break;
                        }

                        case OperatorNode::BIT_AND:
                        {
                            literal.U16 = left->U16 & right->U16;
                            break;
                        }
 
                        case OperatorNode::BIT_OR:
                        {
                            literal.U16 = left->U16 | right->U16;
                            break;
                        }                   

                        case OperatorNode::BIT_XOR:
                        {
                            literal.U16 = left->U16 ^ right->U16;
                            break;
                        }

                        case OperatorNode::BIT_COMPLEMENT:
                        {
                            literal.U16 = ~left->U16;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U16 > right->U16;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U16 >= right->U16;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U16 < right->U16;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U16 <= right->U16;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U16 == right->U16;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U16 != right->U16;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::UINT32)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.U32 = left->U32 + right->U32;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.U32 = left->U32 - right->U32;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.U32 = left->U32 * right->U32;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.U32 = left->U32 / right->U32;
                            break;
                        }

                        case OperatorNode::AR_MODULUS:
                        {
                            literal.U32 = left->U32 % right->U32;
                            break;
                        }

                        case OperatorNode::BIT_LEFT_SHIFT:
                        {
                            literal.U32 = left->U32 << right->U32;
                            break;
                        }

                        case OperatorNode::BIT_RIGHT_SHIFT:
                        {
                            literal.U32 = left->U32 >> right->U32;
                            break;
                        }

                        case OperatorNode::BIT_AND:
                        {
                            literal.U32 = left->U32 & right->U32;
                            break;
                        }
 
                        case OperatorNode::BIT_OR:
                        {
                            literal.U32 = left->U32 | right->U32;
                            break;
                        }                   

                        case OperatorNode::BIT_XOR:
                        {
                            literal.U32 = left->U32 ^ right->U32;
                            break;
                        }

                        case OperatorNode::BIT_COMPLEMENT:
                        {
                            literal.U32 = ~left->U32;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U32 > right->U32;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U32 >= right->U32;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U32 < right->U32;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U32 <= right->U32;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U32 == right->U32;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U32 != right->U32;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::UINT64)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.U64 = left->U64 + right->U64;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.U64 = left->U64 - right->U64;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.U64 = left->U64 * right->U64;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.U64 = left->U64 / right->U64;
                            break;
                        }

                        case OperatorNode::AR_MODULUS:
                        {
                            literal.U64 = left->U64 % right->U64;
                            break;
                        }

                        case OperatorNode::BIT_LEFT_SHIFT:
                        {
                            literal.U64 = left->U64 << right->U64;
                            break;
                        }

                        case OperatorNode::BIT_RIGHT_SHIFT:
                        {
                            literal.U64 = left->U64 >> right->U64;
                            break;
                        }

                        case OperatorNode::BIT_AND:
                        {
                            literal.U64 = left->U64 & right->U64;
                            break;
                        }
 
                        case OperatorNode::BIT_OR:
                        {
                            literal.U64 = left->U64 | right->U64;
                            break;
                        }                   

                        case OperatorNode::BIT_XOR:
                        {
                            literal.U64 = left->U64 ^ right->U64;
                            break;
                        }

                        case OperatorNode::BIT_COMPLEMENT:
                        {
                            literal.U64 = ~left->U64;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U64 > right->U64;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U64 >= right->U64;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U64 < right->U64;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U64 <= right->U64;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U64 == right->U64;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->U64 != right->U64;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::INT8)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.I8 = left->I8 + right->I8;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.I8 = left->I8 - right->I8;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.I8 = left->I8 * right->I8;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.I8 = left->I8 / right->I8;
                            break;
                        }

                        case OperatorNode::AR_MODULUS:
                        {
                            literal.I8 = left->I8 % right->I8;
                            break;
                        }

                        case OperatorNode::BIT_LEFT_SHIFT:
                        {
                            literal.I8 = left->I8 << right->I8;
                            break;
                        }

                        case OperatorNode::BIT_RIGHT_SHIFT:
                        {
                            literal.I8 = left->I8 >> right->I8;
                            break;
                        }

                        case OperatorNode::BIT_AND:
                        {
                            literal.I8 = left->I8 & right->I8;
                            break;
                        }
 
                        case OperatorNode::BIT_OR:
                        {
                            literal.I8 = left->I8 | right->I8;
                            break;
                        }                   

                        case OperatorNode::BIT_XOR:
                        {
                            literal.I8 = left->I8 ^ right->I8;
                            break;
                        }

                        case OperatorNode::BIT_COMPLEMENT:
                        {
                            literal.I8 = ~left->I8;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I8 > right->I8;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I8 >= right->I8;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I8 < right->I8;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I8 <= right->I8;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I8 == right->I8;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I8 != right->I8;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::INT16)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.I16 = left->I16 + right->I16;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.I16 = left->I16 - right->I16;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.I16 = left->I16 * right->I16;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.I16 = left->I16 / right->I16;
                            break;
                        }

                        case OperatorNode::AR_MODULUS:
                        {
                            literal.I16 = left->I16 % right->I16;
                            break;
                        }

                        case OperatorNode::BIT_LEFT_SHIFT:
                        {
                            literal.I16 = left->I16 << right->I16;
                            break;
                        }

                        case OperatorNode::BIT_RIGHT_SHIFT:
                        {
                            literal.I16 = left->I16 >> right->I16;
                            break;
                        }

                        case OperatorNode::BIT_AND:
                        {
                            literal.I16 = left->I16 & right->I16;
                            break;
                        }
 
                        case OperatorNode::BIT_OR:
                        {
                            literal.I16 = left->I16 | right->I16;
                            break;
                        }                   

                        case OperatorNode::BIT_XOR:
                        {
                            literal.I16 = left->I16 ^ right->I16;
                            break;
                        }

                        case OperatorNode::BIT_COMPLEMENT:
                        {
                            literal.I16 = ~left->I16;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I16 > right->I16;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I16 >= right->I16;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I16 < right->I16;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I16 <= right->I16;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I16 == right->I16;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I16 != right->I16;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::INT32)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.I32 = left->I32 + right->I32;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.I32 = left->I32 - right->I32;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.I32 = left->I32 * right->I32;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.I32 = left->I32 / right->I32;
                            break;
                        }

                        case OperatorNode::AR_MODULUS:
                        {
                            literal.I32 = left->I32 % right->I32;
                            break;
                        }

                        case OperatorNode::BIT_LEFT_SHIFT:
                        {
                            literal.I32 = left->I32 << right->I32;
                            break;
                        }

                        case OperatorNode::BIT_RIGHT_SHIFT:
                        {
                            literal.I32 = left->I32 >> right->I32;
                            break;
                        }

                        case OperatorNode::BIT_AND:
                        {
                            literal.I32 = left->I32 & right->I32;
                            break;
                        }
 
                        case OperatorNode::BIT_OR:
                        {
                            literal.I32 = left->I32 | right->I32;
                            break;
                        }                   

                        case OperatorNode::BIT_XOR:
                        {
                            literal.I32 = left->I32 ^ right->I32;
                            break;
                        }

                        case OperatorNode::BIT_COMPLEMENT:
                        {
                            literal.I32 = ~left->I32;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I32 > right->I32;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I32 >= right->I32;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I32 < right->I32;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I32 <= right->I32;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I32 == right->I32;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I32 != right->I32;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::INT64)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.I64 = left->I64 + right->I64;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.I64 = left->I64 - right->I64;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.I64 = left->I64 * right->I64;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.I64 = left->I64 / right->I64;
                            break;
                        }

                        case OperatorNode::AR_MODULUS:
                        {
                            literal.I64 = left->I64 % right->I64;
                            break;
                        }

                        case OperatorNode::BIT_LEFT_SHIFT:
                        {
                            literal.I64 = left->I64 << right->I64;
                            break;
                        }

                        case OperatorNode::BIT_RIGHT_SHIFT:
                        {
                            literal.I64 = left->I64 >> right->I64;
                            break;
                        }

                        case OperatorNode::BIT_AND:
                        {
                            literal.I64 = left->I64 & right->I64;
                            break;
                        }
 
                        case OperatorNode::BIT_OR:
                        {
                            literal.I64 = left->I64 | right->I64;
                            break;
                        }                   

                        case OperatorNode::BIT_XOR:
                        {
                            literal.I64 = left->I64 ^ right->I64;
                            break;
                        }

                        case OperatorNode::BIT_COMPLEMENT:
                        {
                            literal.I64 = ~left->I64;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I64 > right->I64;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I64 >= right->I64;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I64 < right->I64;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I64 <= right->I64;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I64 == right->I64;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->I64 != right->I64;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::FLOAT32)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.F32 = left->F32 + right->F32;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.F32 = left->F32 - right->F32;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.F32 = left->F32 * right->F32;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.F32 = left->F32 / right->F32;
                            break;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F32 > right->F32;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F32 >= right->F32;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F32 < right->F32;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F32 <= right->F32;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F32 == right->F32;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F32 != right->F32;
                            break;
                        }
                    }
                }
                else if (literal.Type == LiteralNode::FLOAT64)
                {
                    switch (op->Type)
                    {
                        case OperatorNode::AR_ADD:
                        {
                            literal.F64 = left->F64 + right->F64;
                            break;
                        }
                        
                        case OperatorNode::AR_SUBTRACT:
                        {
                            literal.F64 = left->F64 - right->F64;
                            break;
                        }

                        case OperatorNode::AR_MULTIPLY:
                        {
                            literal.F64 = left->F64 * right->F64;
                            break;
                        }

                        case OperatorNode::AR_DIVIDE:
                        {
                            literal.F64 = left->F64 / right->F64;
                            break;
                        }

                        case OperatorNode::CMP_GREATER:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F64 > right->F64;
                            break;
                        }

                        case OperatorNode::CMP_GREATER_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F64 >= right->F64;
                            break;
                        }

                        case OperatorNode::CMP_LESS:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F64 < right->F64;
                            break;
                        }

                        case OperatorNode::CMP_LESS_OR_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F64 <= right->F64;
                            break;
                        }

                        case OperatorNode::CMP_IS_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F64 == right->F64;
                            break;
                        }

                        case OperatorNode::CMP_NOT_EQUAL:
                        {
                            literal.Type = LiteralNode::BOOL;
                            literal.Bool = left->F64 != right->F64;
                            break;
                        }
                    }
                }

                return literal;
            }

            IRModuleNode * ConstantFolding(IRModuleNode * ir)
            {
                for (auto & function : ir->Functions)
                {
                    std::vector<std::vector<IRStatementNode>::iterator> iteratorsToRemove;

                    for (auto iter = function.Statements.begin(); iter != function.Statements.end(); iter++)
                    {
                        if (IsConstant(&iter->Operator))
                        {
                            LiteralNode literal = FoldConstants(&iter->Operator);

                            iteratorsToRemove.emplace_back(iter);

                            auto nextIter = iter + 1;

                            auto left = dynamic_cast<VariableNode *>(nextIter->Operator.Left);
                            auto right = dynamic_cast<VariableNode *>(nextIter->Operator.Right);

                            if (left && left->Name == "")
                            {
                                nextIter->Operator.Left = new LiteralNode(literal);
                            }
                            
                            if (right && right->Name == "")
                            {
                                nextIter->Operator.Right = new LiteralNode(literal);
                            }
                        }
                    }

                    for (size_t i = 0; i < iteratorsToRemove.size(); i++)
                    {
                        function.Statements.erase(iteratorsToRemove[i] - i);
                    }
                }

                return ir;
            }
        }
    }
}
