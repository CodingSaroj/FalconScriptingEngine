#include "Generator.hpp"

namespace Falcon
{
    namespace Assembler
    {
        Generator::Generator(ASTNode * node)
            : m_Node(node)
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
                    if (type == OpCode::UMOV8 || type == OpCode::IMOV8)
                    {
                        m_Bytecode += inst->Args[i].Char;
                    }
                    else if (type == OpCode::UMOV16 || type == OpCode::IMOV16)
                    {
                        m_Bytecode.append((char *)&inst->Args[i].Uint, 2);
                    }
                    else if (type == OpCode::UMOV32 || type == OpCode::IMOV32 || type == OpCode::FMOV32 || (type >= OpCode::LOAD8 && type <= OpCode::LODREF) || type == OpCode::CALL || type == OpCode::RET || type == OpCode::POPNUL)
                    {
                        m_Bytecode.append((char *)&inst->Args[i].Uint, 4);
                    }
                    else if (type == OpCode::UMOV64 || type == OpCode::IMOV64 || type == OpCode::FMOV64 || type == OpCode::JMP || type == OpCode::JMT || type == OpCode::JMF)
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
        }

        std::string Generator::generate()
        {
            if (auto code = dynamic_cast<CodeSectionNode *>(m_Node)) { generateCodeSection(code); }

            std::string data = generateSymbolTable() + m_Bytecode;
            uint64_t size = data.size();

            data = std::string((char *)&size, 8) + data;

            return "FLCN" + data + '\377';
        }
    }
}
