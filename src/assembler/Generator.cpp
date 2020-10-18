#include "Generator.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Generator::Generator(ASTNode * node, bool debug)
            : m_Node(node), m_Debug(debug)
        {
        }

        std::string Generator::GenerateSymbolTable()
        {
            std::string symbols;

            for (auto sym : m_Symbols)
            {
                symbols += sym.first;
                symbols += '\0';

                Endian::SystemToLittle(sym.second);

                symbols.append((char *)&sym.second, 8);
            }

            return symbols;
        }

        void Generator::GenerateInstruction(InstructionNode * inst)
        {
            OpCode::OpCode type = (OpCode::OpCode)255;

            for (int i = 0; i < OpCode::s_Names.size(); i++)
            {
                if (OpCode::s_Names[i] == inst->Inst)
                {
                    type = (OpCode::OpCode)i;
                    break;
                }
            }

            m_CodeSection += type;

            for (int i = 0; i < inst->Args.size(); i++)
            {
                if (inst->Args[i].Str)
                {
                    if (type >= OpCode::JMP && type <= OpCode::JMF)
                    {
                        m_UnresolvedSymbols.emplace_back(m_CodeSection.size(), *inst->Args[i].Str);

                        for (int i = 0; i < 8; i++)
                        {
                            m_CodeSection += '\0';
                        }
                    }
                    else
                    {
                        m_CodeSection += *inst->Args[i].Str;
                        m_CodeSection += '\0';
                    }
                }
                else if (inst->Args[i].Register)
                {
                    RegisterType::RegisterType type = (RegisterType::RegisterType)255;
                    uint8_t & typeU8 = *(uint8_t *)&type;

                    bool at = false, ref = false;
                    std::string bare;

                    if (inst->Args[i].Register->Name[0] == '@')
                    {
                        at = true;
                        bare = inst->Args[i].Register->Name.substr(1);
                    }
                    else if (inst->Args[i].Register->Name[0] == '[' && inst->Args[i].Register->Name[inst->Args[i].Register->Name.size() - 1] == ']')
                    {
                        ref = true;
                        bare = inst->Args[i].Register->Name.substr(1, inst->Args[i].Register->Name.size() - 2);
                    }
                    else
                    {
                        bare = inst->Args[i].Register->Name;
                    }

                    for (int j = 0; j < RegisterType::s_Names.size(); j++)
                    {
                        if (RegisterType::s_Names[j] == bare)
                        {
                            type = (RegisterType::RegisterType)j;
                            break;
                        }
                    }

                    if (at)
                    {
                        typeU8 |= 0b00010000;
                    }

                    if (ref)
                    {
                        typeU8 |= 0b00100000;
                    }

                    m_CodeSection += type;
                }
                else
                {
                    AtomNode::UnionType data = inst->Args[i].GetUnion();

                    if (type == OpCode::MOV8)
                    {
                        m_CodeSection += data.Char;
                    }
                    else if (type == OpCode::MOV16)
                    {
                        Endian::SystemToLittle(*(uint16_t *)&data.Uint);
                        
                        m_CodeSection.append((char *)&data.Uint, 2);
                    }
                    else if (type == OpCode::MOV32
                            || (type >= OpCode::LOAD8 && type <= OpCode::GLODREF)
                            || type == OpCode::CALL
                            || type == OpCode::RET
                            || type == OpCode::POPNUL)
                    {
                        if (inst->Args[i].Float)
                        {
                            // Decrease float precision to 32-bit.
                            *(float *)&data.Float = data.Float;
                        }

                        Endian::SystemToLittle(*(uint32_t *)&data.Uint);

                        m_CodeSection.append((char *)&data.Uint, 4);
                    }
                    else if (type == OpCode::MOV64 || type == OpCode::JMP || type == OpCode::JMT || type == OpCode::JMF)
                    {
                        Endian::SystemToLittle(*(uint64_t *)&data.Uint);

                        m_CodeSection.append((char *)&data.Uint, 8);
                    }
                }
            }
        }

        void Generator::GenerateRoutine(RoutineNode * routine)
        {
            m_CodeSection += (char)OpCode::FUNC;

            for (auto & label : routine->Labels)
            {
                m_LabelAddresses[label.Name] = m_CodeSection.size() - 12;

                for (auto & inst : label.Instructions)
                {
                    GenerateInstruction(&inst);
                }
            }

            m_CodeSection += OpCode::FUNC;
        }

        void Generator::GenerateCodeSection(CodeSectionNode * code)
        {
            m_CodeSection += "CODE";

            uint64_t sizeStart = m_CodeSection.size();

            for (int i = 0; i < 8; i++)
            {
                m_CodeSection += '\0';
            }

            uint64_t codeSectionStart = m_CodeSection.size();

            for (auto & routine : code->Routines)
            {
                m_Symbols[routine.Name] = m_CodeSection.size() - 12;

                GenerateRoutine(&routine);
            }

            for (auto sym : m_UnresolvedSymbols)
            {
                uint64_t location = sym.first;
                std::string name = sym.second;

                uint64_t address = m_LabelAddresses[name];

                Endian::SystemToLittle(address);

                for (int i = 0; i < 8; i++)
                {
                    m_CodeSection[location + i] = ((char *)&address)[i];
                }
            }

            m_CodeSection.insert(codeSectionStart, GenerateSymbolTable());
            
            uint64_t codeSectionSize = (m_CodeSection.size() - codeSectionStart) + 1;

            Endian::SystemToLittle(codeSectionSize);

            for (int i = 0; i < 8; i++)
            {
                m_CodeSection[sizeStart + i] = ((char *)&codeSectionSize)[i];
            }

            m_CodeSection += '\377';
        }

        void Generator::GenerateDebugRoutine(DebugRoutineNode * routine)
        {
            m_DebugSection += routine->Name;
            m_DebugSection += '\0';

            m_DebugSection += 'M';
            m_DebugSection += routine->MetaData.Signature;
            m_DebugSection += '\0';
            
            Endian::SystemToLittle(routine->MetaData.StartLine);
            Endian::SystemToLittle(routine->MetaData.EndLine);

            for (int i = 0; i < 16; i++)
            {
                m_DebugSection += ((char *)&routine->MetaData.StartLine)[i];
            }

            m_DebugSection += 'L';

            for (auto lineMap : routine->LineMaps)
            {
                Endian::SystemToLittle(lineMap.StartLocation);
                Endian::SystemToLittle(lineMap.LineNumber);

                for (int i = 0; i < 16; i++)
                {
                    m_DebugSection += ((char *)&lineMap.StartLocation)[i];
                }

                m_DebugSection += lineMap.LineData;
                m_DebugSection += '\0';
            }

            m_DebugSection += 'V';

            for (auto localVar : routine->LocalVariables)
            {
                m_DebugSection += localVar.Name;
                m_DebugSection += '\0';

                m_DebugSection += localVar.Type;
                m_DebugSection += '\0';
                
                Endian::SystemToLittle(localVar.StackOffset);

                for (int i = 0; i < 8; i++)
                {
                    m_DebugSection += ((char *)&localVar.StackOffset)[i];
                }
            }

            m_DebugSection += 'E';
        }

        void Generator::GenerateDebugSection(DebugSectionNode * dbg)
        {
            m_DebugSection += "DEBG";

            uint64_t sizeStart = m_DebugSection.size();

            for (int i = 0; i < 8; i++)
            {
                m_DebugSection += '\0';
            }

            uint64_t debugSectionStart = m_DebugSection.size();

            for (auto & routine : dbg->Routines)
            {
                GenerateDebugRoutine(&routine);
            }

            uint64_t debugSectionSize = m_DebugSection.size() - debugSectionStart;

            Endian::SystemToLittle(debugSectionSize);

            for (int i = 0; i < 8; i++)
            {
                m_DebugSection[sizeStart + i] = ((char *)&debugSectionSize)[i];
            }
        }
        
        void Generator::GenerateReflectionAttribute(ReflectionAttributeNode * attrib)
        {
            m_ReflectionSection += 'T';

            m_ReflectionSection += attrib->Name;
            m_ReflectionSection += '\0';

            for (auto & attrName : attrib->Attributes)
            {
                m_ReflectionSection += attrName;
                m_ReflectionSection += '\0';
            }

            m_ReflectionSection += 'E';
        }

        void Generator::GenerateReflectionFunction(ReflectionFunctionNode * function)
        {
            m_ReflectionSection += 'F';

            m_ReflectionSection += function->ReturnType;
            m_ReflectionSection += '\0';

            m_ReflectionSection += function->Name;
            m_ReflectionSection += '\0';

            for (auto & param : function->Parameters)
            {
                m_ReflectionSection += param;
                m_ReflectionSection += '\0';
            }

            m_ReflectionSection += 'E';
        }

        void Generator::GenerateReflectionStructure(ReflectionStructureNode * structure)
        {
            m_ReflectionSection += 'S';

            m_ReflectionSection += structure->Name;
            m_ReflectionSection += '\0';

            for (auto & member : structure->Members)
            {
                m_ReflectionSection += member.first;
                m_ReflectionSection += '\0';

                m_ReflectionSection += member.second;
                m_ReflectionSection += '\0';
            }

            m_ReflectionSection += 'E';
        }

        void Generator::GenerateReflectionAlias(ReflectionAliasNode * alias)
        {
            m_ReflectionSection += 'A';

            m_ReflectionSection += alias->Name;
            m_ReflectionSection += '\0';

            m_ReflectionSection += alias->Base;
            m_ReflectionSection += '\0';

            m_ReflectionSection += 'E';
        }

        void Generator::GenerateReflectionSection(ReflectionSectionNode * refl)
        {
            m_ReflectionSection += "REFL";

            uint64_t sizeStart = m_ReflectionSection.size();

            for (int i = 0; i < 8; i++)
            {
                m_ReflectionSection += '\0';
            }

            uint64_t reflectionSectionStart = m_ReflectionSection.size();

            for (auto & attrib : refl->Attributes)
            {
                GenerateReflectionAttribute(&attrib);
            }

            for (auto & function : refl->Functions)
            {
                GenerateReflectionFunction(&function);
            }

            for (auto & structure : refl->Structures)
            {
                GenerateReflectionStructure(&structure);
            }
            
            for (auto & alias : refl->Aliases)
            {
                GenerateReflectionAlias(&alias);
            }

            uint64_t reflectionSectionSize = m_ReflectionSection.size() - reflectionSectionStart;

            Endian::SystemToLittle(reflectionSectionSize);

            for (int i = 0; i < 8; i++)
            {
                m_ReflectionSection[sizeStart + i] = ((char *)&reflectionSectionSize)[i];
            }
        }
       
        void Generator::GenerateModule(ModuleNode * module)
        {
            GenerateCodeSection(&module->CodeSection);
            m_Bytecode += m_CodeSection;

            if (m_Debug)
            {
                GenerateDebugSection(&module->DebugSection);
                m_Bytecode += m_DebugSection;
            }

            GenerateReflectionSection(&module->ReflectionSection);
            m_Bytecode += m_ReflectionSection;
        }

        std::string Generator::Generate()
        {
            if (auto module = dynamic_cast<ModuleNode *>(m_Node))
            {
                GenerateModule(module);
            }

            return "FALI" + m_Bytecode;
        }
    }
}
