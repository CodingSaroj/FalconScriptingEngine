#include "Serialize.hpp"

namespace Falcon
{
    namespace Assembler
    {
        void Serialize(Token token)
        {
                switch (token.Type)
            {
                case TokenType::SECTION:
                    std::cout<<"Section:\t\t"<<token.Str<<"\n";
                    break;

                case TokenType::INSTRUCTION:
                    std::cout<<"Instruction:\t\t"<<token.Str<<"\n";
                    break;
        
                case TokenType::REGISTER:
                    std::cout<<"Register:\t\t"<<token.Str<<"\n";
                    break;
        
                case TokenType::IDENTIFIER:
                    std::cout<<"Identifier:\t\t"<<token.Str<<"\n";
                    break;
        
                case TokenType::CHAR:
                    std::cout<<"Character:\t\t"<<token.Char<<"\n";
                    break;
        
                case TokenType::INT:
                    std::cout<<"Integer:\t\t"<<token.Int<<"\n";
                    break;
                    
                case TokenType::UINT:
                    std::cout<<"UnsignedInteger:\t"<<token.Uint<<"\n";
                    break;
        
                case TokenType::FLOAT:
                    std::cout<<"FloatingPoint:\t\t"<<token.Float<<"\n";
                    break;
        
                case TokenType::NEWLINE:
                    std::cout<<"NewLine\n";
                    break;
        
                default:
                    std::cout<<"Arbitarory:\t\t"<<(char)token.Type<<"\n";
                    break;
            }
        }
        
        void Serialize(ASTNode * node, std::string padding)
        {
            if (!node)
            {
                return;
            }
        
            if (AtomNode * atom = dynamic_cast<AtomNode *>(node))
            {
                std::cout<<padding;
        
                switch (atom->Type)
                {
                    case AtomNode::AtomType::IDENTIFIER:
                        std::cout<<"IdentifierNode:\t\t"<<atom->Str<<"\n";
                        break;

                    case AtomNode::AtomType::REGISTER:
                        std::cout<<"RegisterNode:\t\t"<<atom->Str<<"\n";
                        break;
        
                    case AtomNode::AtomType::CHAR:
                        std::cout<<"CharNode:\t\t"<<atom->Char<<"\n";
                        break;
        
                    case AtomNode::AtomType::UINT:
                        std::cout<<"UintNode:\t\t"<<atom->Uint<<"\n";
                        break;
                    
                    case AtomNode::AtomType::INT:
                        std::cout<<"IntNode:\t\t"<<atom->Int<<"\n";
                        break;
                    
                    case AtomNode::AtomType::FLOAT:
                        std::cout<<"FloatNode:\t\t"<<atom->Float<<"\n";
                        break;
                }
            }
            else if (InstructionNode * inst = dynamic_cast<InstructionNode *>(node))
            {
                std::cout<<padding;
        
                std::cout<<"InstructionNode:\t"<<inst->Inst<<"\n";
        
                for (AtomNode & arg : inst->Args)
                {
                    Serialize(&arg, padding + "\t");
                }
            }
            else if (LabelNode * label = dynamic_cast<LabelNode *>(node))
            {
                std::cout<<padding;

                std::cout<<"LabelNode:\t"<<label->Name<<"\n";
                
                for (InstructionNode & inst : label->Instructions)
                {
                    Serialize(&inst, padding + "\t");
                }
            }
            else if (RoutineNode * routine = dynamic_cast<RoutineNode *>(node))
            {
                std::cout<<padding;
        
                std::cout<<"RoutineNode:\t"<<routine->Name<<"\n";
        
                for (LabelNode & label : routine->Labels)
                {
                    Serialize(&label, padding + "\t");
                }
            }
            else if (CodeSectionNode * code = dynamic_cast<CodeSectionNode *>(node))
            {
                std::cout<<"CodeSectionNode:\n";
        
                for (RoutineNode & routine : code->Routines)
                {
                    Serialize(&routine, "\t");
                }
            }
        }
    }
}
