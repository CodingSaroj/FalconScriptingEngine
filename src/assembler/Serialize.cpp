/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "FalconPCH.hpp"

#include "Serialize.hpp"

namespace Falcon
{
    namespace Assembler
    {
        void Serialize(const Token & token)
        {
            if (token.Section)
            {
                std::cout<<"Section:\n";
            }
            else if (token.Instruction)
            {
                std::cout<<Format("Instruction: {}\n", token.Instruction->Name);
            }
            else if (token.Register)
            {
                std::cout<<Format("Register: {}\n", token.Register->Name);
            }
            else if (token.Char)
            {
                std::cout<<Format("Character: {}\n", *token.Char);
            }
            else if (token.Uint)
            {
                std::cout<<Format("UnsignedInteger: {}\n", *token.Uint);
            }
            else if (token.Int)
            {
                std::cout<<Format("Integer: {}\n", *token.Int);
            }
            else if (token.Float)
            {
                std::cout<<Format("Float: {}\n", *token.Float);
            }
            else if (token.Str)
            {
                std::cout<<Format("Identifier: {}\n", *token.Str);
            }
            else if (token.Arbitarory)
            {
                std::cout<<Format("Arbitarory: {}\n", token.Arbitarory->Type);
            }
            else if (token.NewLine)
            {
                std::cout<<"Newline\n";
            }
        }
        
        void Serialize(ASTNode * node, const std::string & padding)
        {
            if (!node)
            {
                return;
            }
        
            if (auto * atom = dynamic_cast<AtomNode *>(node))
            {
                std::cout<<padding;
        
                if (atom->Char)
                {
                    std::cout<<Format("Character: {}\n", *atom->Char);
                }
                else if (atom->Uint)
                {
                    std::cout<<Format("UnsignedInteger: {}\n", *atom->Char);
                }
                else if (atom->Int)
                {
                    std::cout<<Format("Integer: {}\n", *atom->Char);
                }
                else if (atom->Float)
                {
                    std::cout<<Format("Float: {}\n", *atom->Char);
                }
                else if (atom->Str)
                {
                    std::cout<<Format("Identifier: {}\n", *atom->Str);
                }
                else if (atom->Register)
                {
                    std::cout<<Format("Float: {}\n", atom->Register->Name);
                }
            }
            else if (auto * inst = dynamic_cast<InstructionNode *>(node))
            {
                std::cout<<Format("{}Instruction: {}\n", padding, inst->Inst);
        
                for (AtomNode & arg : inst->Args)
                {
                    Serialize(&arg, padding + "\t");
                }
            }
            else if (auto * label = dynamic_cast<LabelNode *>(node))
            {
                std::cout<<Format("{}Label: {}\n", padding, label->Name);
                
                for (InstructionNode & inst : label->Instructions)
                {
                    Serialize(&inst, padding + "\t");
                }
            }
            else if (auto * routine = dynamic_cast<RoutineNode *>(node))
            {
                std::cout<<Format("{}Routine: {}\n", padding, routine->Name);
        
                for (LabelNode & label : routine->Labels)
                {
                    Serialize(&label, padding + "\t");
                }
            }
            else if (auto * code = dynamic_cast<CodeSectionNode *>(node))
            {
                std::cout<<Format("{}Routine:\n", padding);
        
                for (RoutineNode & routine : code->Routines)
                {
                    Serialize(&routine, padding + "\t");
                }
            }
            else if (auto * meta = dynamic_cast<DebugMetaNode *>(node))
            {
                std::cout<<Format("{}DebugMeta:\n", padding);
                std::cout<<Format("{}{}\n", padding + "\t", meta->Signature);
            }
            else if (auto * lineMap = dynamic_cast<DebugLineMapNode *>(node))
            {
                std::cout<<Format("{}LineMapNode:\n", padding);
                std::cout<<Format("{}StartLocation: {}\n", padding + "\t", lineMap->StartLocation);
                std::cout<<Format("{}LineNumber: {}\n", padding + "\t", lineMap->StartLocation);
                std::cout<<Format("{}LineData: {}\n", padding + "\t", lineMap->StartLocation);
            }
            else if (auto * localVar = dynamic_cast<DebugLocalVarNode *>(node))
            {
                std::cout<<Format("{}DebugLocalVariable:\n", padding);

                std::cout<<Format("{}{} {} @ fp+{}\n", padding + "\t", localVar->Type, localVar->Name, localVar->StackOffset);
            }
            else if (auto * routine = dynamic_cast<DebugRoutineNode *>(node))
            {
                std::cout<<Format("{}DebugRoutine: {}\n", padding, routine->Name);

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
                std::cout<<Format("{}DebugSection:\n", padding);

                for (auto & routine : debug->Routines)
                {
                    Serialize(&routine, padding + "\t");
                }
            }
            else if (auto * attrib = dynamic_cast<ReflectionAttributeNode *>(node))
            {
                std::cout<<Format("{}ReflectionAttribute:\n", padding);

                std::cout<<Format("{}Target: {}\n", padding + "\t", attrib->Name);

                for (auto attr : attrib->Attributes)
                {
                    std::cout<<Format("{}Attribute: {}\n", padding + "\t", attr);
                }
            }
            else if (auto * function = dynamic_cast<ReflectionFunctionNode *>(node))
            {
                std::cout<<Format("{}ReflectionFunction: {} {}(", padding, function->ReturnType, function->Name);

                for (auto param : function->Parameters)
                {
                    std::cout<<Format("{}, ", param);
                }

                std::cout<<"\b\b)\n";
            }
            else if (auto * structure = dynamic_cast<ReflectionStructureNode *>(node))
            {
                std::cout<<Format("{}ReflectionStructure: {}\n", padding, structure->Name);

                for (auto member : structure->Members)
                {
                    std::cout<<Format("{}Member: {} {}\n", padding + "\t", member.first, member.second);
                }
            }
            else if (auto * alias = dynamic_cast<ReflectionAliasNode *>(node))
            {
                std::cout<<Format("{}ReflectionAlias: {} => {}\n", padding, alias->Name, alias->Base);
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
