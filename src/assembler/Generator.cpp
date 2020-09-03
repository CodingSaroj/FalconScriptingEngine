#include "Generator.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Generator::Generator(ASTNode * node, bool debug)
            : m_Node(node), m_Debug(debug)
        {
        }

        std::string Generator::generateSymbolTable()
        {
            std::string symbols;

            for (auto sym : m_Symbols)
            {
                symbols += sym.first;
                symbols += '\0';
                symbols.append((char *)&sym.second, 8);
            }

            return symbols;
        }

        void Generator::generateInstruction(InstructionNode * inst)
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

            m_Bytecode += type;

            for (int i = 0; i < inst->Args.size(); i++)
            {
                if (inst->Args[i].Type == AtomNode::AtomType::IDENTIFIER)
                {
                    if (type >= OpCode::JMP && type <= OpCode::JMF)
                    {
                        m_UnresolvedSymbols.emplace_back(m_Bytecode.size(), inst->Args[i].Str);

                        for (int i = 0; i < 8; i++)
                        {
                            m_Bytecode += '\0';
                        }
                    }
                    else
                    {
                        m_Bytecode += inst->Args[i].Str;
                        m_Bytecode += '\0';
                    }
                }
                else if (inst->Args[i].Type == AtomNode::AtomType::REGISTER)
                {
                    RegisterType::RegisterType type = (RegisterType::RegisterType)255;
                    uint8_t & typeU8 = *(uint8_t *)&type;

                    bool at = false, ref = false;
                    std::string bare;

                    if (inst->Args[i].Str[0] == '@')
                    {
                        at = true;
                        bare = inst->Args[i].Str.substr(1);
                    }
                    else if (inst->Args[i].Str[0] == '[' && inst->Args[i].Str[inst->Args[i].Str.size() - 1] == ']')
                    {
                        ref = true;
                        bare = inst->Args[i].Str.substr(1, inst->Args[i].Str.size() - 2);
                    }
                    else
                    {
                        bare = inst->Args[i].Str;
                    }

                    for (int j = 0; j < RegisterType::s_Names.size(); j++)
                    {
                        if (RegisterType::s_Names[j] == bare)
                        {
                            type = (RegisterType::RegisterType)j;
                            break;
                        }
                    }

                    if (at) { typeU8 |= 0b00000100; }
                    if (ref) { typeU8 |= 0b00001000; }

                    m_Bytecode += type;
                }
                else
                {
                    if (type == OpCode::MOV8)
                    {
                        m_Bytecode += inst->Args[i].Char;
                    }
                    else if (type == OpCode::MOV16)
                    {
                        m_Bytecode.append((char *)&inst->Args[i].Uint, 2);
                    }
                    else if (type == OpCode::MOV32 || (type >= OpCode::LOAD8 && type <= OpCode::LODREF) || type == OpCode::CALL || type == OpCode::RET || type == OpCode::POPNUL)
                    {
                        m_Bytecode.append((char *)&inst->Args[i].Uint, 4);
                    }
                    else if (type == OpCode::MOV64 || type == OpCode::JMP || type == OpCode::JMT || type == OpCode::JMF)
                    {
                        m_Bytecode.append((char *)&inst->Args[i].Uint, 8);
                    }
                }
            }
        }

        void Generator::generateRoutine(RoutineNode * routine)
        {
            m_Bytecode += (char)OpCode::FUNC;

            for (auto & label : routine->Labels)
            {
                m_LabelAddresses[label.Name] = m_Bytecode.size();

                for (auto & inst : label.Instructions)
                {
                    generateInstruction(&inst);
                }
            }

            m_Bytecode += OpCode::FUNC;
        }

        void Generator::generateCodeSection(CodeSectionNode * code)
        {
            m_Bytecode += "CODE";

            uint64_t sizeStart = m_Bytecode.size();

            for (int i = 0; i < 8; i++)
            {
                m_Bytecode += '\0';
            }

            uint64_t codeSectionStart = m_Bytecode.size();

            for (auto & routine : code->Routines)
            {
                m_Symbols[routine.Name] = m_Bytecode.size();

                generateRoutine(&routine);
            }

            for (auto sym : m_UnresolvedSymbols)
            {
                uint64_t location = sym.first;
                std::string name = sym.second;

                uint64_t address = m_LabelAddresses[name];

                for (int i = 0; i < 8; i++)
                {
                    m_Bytecode[location + i] = ((char *)&address)[i];
                }
            }

            m_Bytecode.insert(codeSectionStart, generateSymbolTable());
            
            uint64_t codeSectionSize = (m_Bytecode.size() - codeSectionStart) + 1;

            for (int i = 0; i < 8; i++)
            {
                m_Bytecode[sizeStart + i] = ((char *)&codeSectionSize)[i];
            }

            m_Bytecode += '\377';
        }

        void Generator::generateDebugRoutine(DebugRoutineNode * routine)
        {
            m_Bytecode += routine->Name;
            m_Bytecode += '\0';

            m_Bytecode += 'M';
            m_Bytecode += routine->MetaData.Signature;
            m_Bytecode += '\0';

            m_Bytecode += 'L';

            for (auto lineMap : routine->LineMaps)
            {
                for (int i = 0; i < 16; i++)
                {
                    m_Bytecode += ((char *)&lineMap.StartLocation)[i];
                }

                m_Bytecode += lineMap.LineData;
                m_Bytecode += '\0';
            }

            m_Bytecode += 'V';

            for (auto localVar : routine->LocalVariables)
            {
                m_Bytecode += localVar.Name;
                m_Bytecode += '\0';

                m_Bytecode += localVar.Type;
                m_Bytecode += '\0';
                
                for (int i = 0; i < 8; i++)
                {
                    m_Bytecode += ((char *)&localVar.StackOffset)[i];
                }
            }

            m_Bytecode += 'E';
        }

        void Generator::generateDebugSection(DebugSectionNode * dbg)
        {
            m_Bytecode += "DEBG";

            uint64_t sizeStart = m_Bytecode.size();

            for (int i = 0; i < 8; i++)
            {
                m_Bytecode += '\0';
            }

            uint64_t debugSectionStart = m_Bytecode.size();

            for (auto & routine : dbg->Routines)
            {
                generateDebugRoutine(&routine);
            }

            uint64_t debugSectionSize = m_Bytecode.size() - debugSectionStart;

            for (int i = 0; i < 8; i++)
            {
                m_Bytecode[sizeStart + i] = ((char *)&debugSectionSize)[i];
            }
        }
        
        void Generator::generateReflectionFunction(ReflectionFunctionNode * function)
        {
            m_Bytecode += "F";

            m_Bytecode += function->ReturnType;
            m_Bytecode += '\0';

            m_Bytecode += function->Name;
            m_Bytecode += '\0';

            for (auto param : function->Parameters)
            {
                m_Bytecode += param;
                m_Bytecode += '\0';
            }
        }

        void Generator::generateReflectionStructure(ReflectionStructureNode * structure)
        {
            m_Bytecode += "S";

            m_Bytecode += structure->Name;
            m_Bytecode += '\0';

            for (auto member : structure->Members)
            {
                m_Bytecode += member.first;
                m_Bytecode += '\0';

                m_Bytecode += member.second;
                m_Bytecode += '\0';
            }
        }

        void Generator::generateReflectionAlias(ReflectionAliasNode * alias)
        {
            m_Bytecode += "A";

            m_Bytecode += alias->Name;
            m_Bytecode += '\0';

            m_Bytecode += alias->Base;
            m_Bytecode += '\0';
        }

        void Generator::generateReflectionSection(ReflectionSectionNode * refl)
        {
            m_Bytecode += "REFL";

            uint64_t sizeStart = m_Bytecode.size();

            for (int i = 0; i < 8; i++)
            {
                m_Bytecode += '\0';
            }

            uint64_t reflectionSectionStart = m_Bytecode.size();

            for (auto & function : refl->Functions)
            {
                generateReflectionFunction(&function);
            }

            for (auto & structure : refl->Structures)
            {
                generateReflectionStructure(&structure);
            }
            
            for (auto & alias : refl->Aliases)
            {
                generateReflectionAlias(&alias);
            }

            uint64_t reflectionSectionSize = m_Bytecode.size() - reflectionSectionStart;

            for (int i = 0; i < 8; i++)
            {
                m_Bytecode[sizeStart + i] = ((char *)&reflectionSectionSize)[i];
            }
        }
       
        void Generator::generateModule(ModuleNode * module)
        {
            generateCodeSection(&module->CodeSection);

            if (m_Debug)
            {
                generateDebugSection(&module->DebugSection);
            }

            generateReflectionSection(&module->ReflectionSection);
        }

        std::string Generator::generate()
        {
            if (auto module = dynamic_cast<ModuleNode *>(m_Node)) { generateModule(module); }

            return "FLCN" + m_Bytecode;
        }
    }
}
