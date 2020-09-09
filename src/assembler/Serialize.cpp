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
                    std::cout<<"Section:    \t"<<token.Str<<"\n";
                    break;
 
                case TokenType::IDENTIFIER:
                    std::cout<<"Identifier:    \t"<<token.Str<<"\n";
                    break;

                case TokenType::INSTRUCTION:
                    std::cout<<"Instruction:    \t"<<token.Str<<"\n";
                    break;
        
                case TokenType::REGISTER:
                    std::cout<<"Register:    \t"<<token.Str<<"\n";
                    break;
        
                case TokenType::CHAR:
                    std::cout<<"Character:    \t"<<token.Char<<"\n";
                    break;
        
                case TokenType::INT:
                    std::cout<<"Integer:    \t"<<token.Int<<"\n";
                    break;
                    
                case TokenType::UINT:
                    std::cout<<"UnsignedInteger:\t"<<token.Uint<<"\n";
                    break;
        
                case TokenType::FLOAT:
                    std::cout<<"FloatingPoint:    \t"<<token.Float<<"\n";
                    break;
        
                case TokenType::NEWLINE:
                    std::cout<<"NewLine\n";
                    break;
        
                default:
                    std::cout<<"Arbitarory:    \t"<<(char)token.Type<<"\n";
                    break;
            }
        }
        
        void Serialize(ASTNode * node, std::string padding)
        {
            if (!node)
            {
                return;
            }
        
            if (auto * atom = dynamic_cast<AtomNode *>(node))
            {
                std::cout<<padding;
        
                switch (atom->Type)
                {
                    case AtomNode::AtomType::IDENTIFIER:
                        std::cout<<"IdentifierNode:    \t"<<atom->Str<<"\n";
                        break;

                    case AtomNode::AtomType::REGISTER:
                        std::cout<<"RegisterNode:    \t"<<atom->Str<<"\n";
                        break;
        
                    case AtomNode::AtomType::CHAR:
                        std::cout<<"CharNode:    \t"<<atom->Char<<"\n";
                        break;
        
                    case AtomNode::AtomType::UINT:
                        std::cout<<"UintNode:    \t"<<atom->Uint<<"\n";
                        break;
                    
                    case AtomNode::AtomType::INT:
                        std::cout<<"IntNode:    \t"<<atom->Int<<"\n";
                        break;
                    
                    case AtomNode::AtomType::FLOAT:
                        std::cout<<"FloatNode:    \t"<<atom->Float<<"\n";
                        break;
                }
            }
            else if (auto * inst = dynamic_cast<InstructionNode *>(node))
            {
                std::cout<<padding;
        
                std::cout<<"InstructionNode:\t"<<inst->Inst<<"\n";
        
                for (AtomNode & arg : inst->Args)
                {
                    Serialize(&arg, padding + "\t");
                }
            }
            else if (auto * label = dynamic_cast<LabelNode *>(node))
            {
                std::cout<<padding;

                std::cout<<"LabelNode:\t"<<label->Name<<"\n";
                
                for (InstructionNode & inst : label->Instructions)
                {
                    Serialize(&inst, padding + "\t");
                }
            }
            else if (auto * routine = dynamic_cast<RoutineNode *>(node))
            {
                std::cout<<padding;
        
                std::cout<<"RoutineNode:\t"<<routine->Name<<"\n";
        
                for (LabelNode & label : routine->Labels)
                {
                    Serialize(&label, padding + "\t");
                }
            }
            else if (auto * code = dynamic_cast<CodeSectionNode *>(node))
            {
                std::cout<<padding<<"CodeSectionNode:\n";
        
                for (RoutineNode & routine : code->Routines)
                {
                    Serialize(&routine, padding + "\t");
                }
            }
            else if (auto * meta = dynamic_cast<DebugMetaNode *>(node))
            {
                std::cout<<padding<<"DebugMetaNode:\n";
                std::cout<<padding + "\t"<<meta->Signature<<"\n";
            }
            else if (auto * lineMap = dynamic_cast<DebugLineMapNode *>(node))
            {
                std::cout<<padding;

                std::cout<<"LineMapNode:\n";
                std::cout<<padding + "\tStartLocation:\t"<<lineMap->StartLocation<<"\n";
                std::cout<<padding + "\tLineNumber:\t"<<lineMap->LineNumber<<"\n";
                std::cout<<padding + "\tLineData:\t\""<<lineMap->LineData<<"\"\n";
            }
            else if (auto * localVar = dynamic_cast<DebugLocalVarNode *>(node))
            {
                std::cout<<padding;

                std::cout<<"LocalVarNode:\n";
                std::cout<<padding + "\t"<<localVar->Name;
                std::cout<<"\n"<<padding + "\t"<<localVar->Type<<"\n";
                std::cout<<padding + "\t"<<localVar->StackOffset<<"\n";
            }
            else if (auto * routine = dynamic_cast<DebugRoutineNode *>(node))
            {
                std::cout<<padding<<"DebugRoutineNode:\t"<<routine->Name<<"\n";

                Serialize(&routine->MetaData, padding + "\t");

                for (auto & lineMap : routine->LineMaps)
                {
                    Serialize(&lineMap, padding + "\t");
                }

                for (auto & localVar : routine->LocalVariables)
                {
                    Serialize(&localVar, padding + "\t");
                }
            }
            else if (auto * debug = dynamic_cast<DebugSectionNode *>(node))
            {
                std::cout<<padding<<"DebugSectionNode:\n";

                for (auto & routine : debug->Routines)
                {
                    Serialize(&routine, padding + "\t");
                }
            }
            else if (auto * attrib = dynamic_cast<ReflectionAttributeNode *>(node))
            {
                std::cout<<padding<<"ReflectionAttributeNode:\n";

                std::cout<<padding + "\tTarget:\t"<<attrib->Name<<"\n";

                for (auto attr : attrib->Attributes)
                {
                    std::cout<<padding + "\tAttribute:\t"<<attr<<"\n";
                }
            }
            else if (auto * function = dynamic_cast<ReflectionFunctionNode *>(node))
            {
                std::cout<<padding<<"ReflectionFunctionNode:\n";

                std::cout<<padding + "\tReturnType:\t"<<function->ReturnType<<"\n";

                std::cout<<padding + "\tName:\t"<<function->Name<<"\n";

                for (auto param : function->Parameters)
                {
                    std::cout<<padding + "\tParam:\t"<<param<<"\n";
                }
            }
            else if (auto * structure = dynamic_cast<ReflectionStructureNode *>(node))
            {
                std::cout<<padding<<"ReflectionStructureNode:\t"<<structure->Name<<"\n";

                for (auto member : structure->Members)
                {
                    std::cout<<padding + "\tMember:\t"<<member.first<<"\t"<<member.second<<"\n";
                }
            }
            else if (auto * alias = dynamic_cast<ReflectionAliasNode *>(node))
            {
                std::cout<<padding<<"ReflectionAliasNode:\n";

                std::cout<<padding + "\tName:\t"<<alias->Name<<"\n";
                std::cout<<padding + "\tBase:\t"<<alias->Base<<"\n";
            }
            else if (auto * refl = dynamic_cast<ReflectionSectionNode *>(node))
            {
                std::cout<<padding<<"ReflectionSectionNode:\n";

                for (auto function : refl->Functions)
                {
                    Serialize(&function, padding + "\t");
                }

                for (auto structure : refl->Structures)
                {
                    Serialize(&structure, padding + "\t");
                }

                for (auto alias : refl->Aliases)
                {
                    Serialize(&alias, padding + "\t");
                }
            } 
            else if (auto * module = dynamic_cast<ModuleNode *>(node))
            {
                std::cout<<"ModuleNode:\n";

                Serialize(&module->CodeSection, "\t");

                Serialize(&module->DebugSection, "\t");

                Serialize(&module->ReflectionSection, "\t");
            }
        }
    }
}
