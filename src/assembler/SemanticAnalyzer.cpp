#include "SemanticAnalyzer.hpp"

namespace Falcon
{
    namespace Assembler
    {
        static std::vector<std::array<uint8_t, 2>> semanticData
        {
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 3}, {0, 3}, {0, 3}, {0, 3},
            {0, 3}, {0, 3}, {0, 3}, {0, 3},

            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 3}, {0, 3}, {0, 3}, {0, 3},

            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},

            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 3}, {0, 3}, {0, 3}, {0, 3},
            {0, 3}, {0, 3}, {0, 3}, {0, 3},

            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 3}, {0, 3}, {0, 3}, {0, 3},

            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},

            {0, 0}, {0, 0},
            {0, 0}, {0, 0},
            {0, 0}, {0, 0},
            {0, 0}, {0, 0},
            {0, 3}, {0, 3},
            {0, 3}, {0, 3},

            {0, 0}, {0, 0},
            {0, 0}, {0, 0},
            {0, 0}, {0, 0},
            {0, 0}, {0, 0},
            {0, 0}, {0, 0},
            {0, 0}, {0, 0},

            {0, 3}, {0, 3}, {0, 3}, {0, 3},
            {0, 3}, {0, 3}, {0, 3}, {0, 3},

            {0, 1}, {0, 2}, {0, 4}, {0, 8},
            {0, 0}, {0, 0}, {0, 0}, {0, 0},

            {9, 3},

            {3, 3}, {3, 3}, {3, 3},

            {0, 4}, {0, 4}, {0, 4}, {0, 4},
            {0, 4},

            {4, 3},
            {4, 3},

            {5, 3}, {5, 3}, {5, 3}, {4, 9},
            {9, 3}, {4, 3}, {3, 3}, {3, 3}
        };

        SemanticAnalyzer::SemanticAnalyzer(ASTNode * node)
            : m_Node(node)
        {
        }

        bool SemanticAnalyzer::findInCodeSection(const std::string & name)
        {
            return std::find_if(m_Code->Routines.begin(), m_Code->Routines.end(),
                    [&name](const RoutineNode & routine)->bool
                    {
                        return routine.Name == name;
                    }) != m_Code->Routines.end();
        }
        
        bool SemanticAnalyzer::findInRoutine(const std::string & name)
        {
            return std::find_if(m_Routine->Labels.begin(), m_Routine->Labels.end(),
                    [&name](const LabelNode & label)->bool
                    {
                        return label.Name == name;
                    }) != m_Routine->Labels.end();
        }

        void SemanticAnalyzer::analyzeInstruction(InstructionNode * inst)
        {
            OpCode::OpCode type = (OpCode::OpCode)255;

            for (uint8_t i = 0; i < OpCode::s_Names.size(); i++)
            {
                if (OpCode::s_Names[i] == inst->Inst)
                {
                    type = (OpCode::OpCode)i;
                }
            }

            uint8_t * fmt = semanticData[type].data();

            if (fmt[0] == 3)
            {
                if (inst->Args.size() != 0)
                {
                    Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects 0 arguments.", inst->Line, inst->Character);
                    exit(2);
                }
            }
            else if (inst->Args.size() != 0)
            {
                if (fmt[0] == 0)
                {
                    if (inst->Args[0].Type != AtomNode::AtomType::REGISTER)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects register as its 1st argument.", inst->Line, inst->Character);
                        exit(2);
                    }
                }
                else if (fmt[0] == 1 || fmt[0] == 2)
                {
                    if (inst->Args[0].Type != AtomNode::AtomType::CHAR && inst->Args[0].Type != AtomNode::AtomType::UINT && inst->Args[0].Type != AtomNode::AtomType::INT)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects char|uint|int as its 1st argument.", inst->Line, inst->Character);
                        exit(2);
                    }
                }
                else if (fmt[0] == 4 || fmt[0] == 8)
                {
                    if (inst->Args[0].Type == AtomNode::AtomType::REGISTER || inst->Args[0].Type == AtomNode::AtomType::IDENTIFIER)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects char|uint|int|float as its 1st argument.", inst->Line, inst->Character);
                        exit(2);
                    }
                }
                else if (fmt[0] == 9)
                {
                    if (inst->Args[0].Type != AtomNode::AtomType::IDENTIFIER)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects identifier as its 2nd argument.", inst->Line, inst->Character);
                        exit(2);
                    }

                    if (type == OpCode::CALL)
                    {
                        if (!findInCodeSection(inst->Args[0].Str))
                        {
                            Log(LogLevel::ERR, "Invalid routine name `" + inst->Args[0].Str + "` in instruction `CALL`.", inst->Line, inst->Character);
                            exit(2);
                        }
                    }
                }
                else if (fmt[0] == 5)
                {
                    if (inst->Args[0].Type == AtomNode::AtomType::REGISTER)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects char|uint|int|float|identifier as its 2nd argument.", inst->Line, inst->Character);
                        exit(2);
                    }
                    else if (inst->Args[0].Type == AtomNode::AtomType::IDENTIFIER)
                    {
                        if (!findInRoutine(inst->Args[0].Str))
                        {
                            Log(LogLevel::ERR, "Invalid label name `" + inst->Args[0].Str + "` in instruction `" + inst->Inst + "`.", inst->Line, inst->Character);
                            exit(2);
                        }
                    }
                }
            }
            else
            {
                Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects 1 or 2 arguments.", inst->Line, inst->Character);
                exit(2);
            }

            if (fmt[1] == 3)
            {
                if (inst->Args.size() == 2)
                {
                    Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects 1 argument.", inst->Line, inst->Character);
                    exit(2); 
                }
            }
            else if (inst->Args.size() == 2)
            {
                if (fmt[1] == 0)
                {
                    if (inst->Args[1].Type != AtomNode::AtomType::REGISTER)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects register as its 2nd argument.", inst->Line, inst->Character);
                        exit(2);
                    }
                }
                else if (fmt[1] == 1 || fmt[1] == 2)
                {
                    if (inst->Args[1].Type != AtomNode::AtomType::CHAR && inst->Args[1].Type != AtomNode::AtomType::UINT && inst->Args[1].Type != AtomNode::AtomType::INT)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects char|uint|int as its 2nd argument.", inst->Line, inst->Character);
                        exit(2);
                    }
                }
                else if (fmt[1] == 4 || fmt[1] == 8)
                {
                    if (inst->Args[1].Type == AtomNode::AtomType::REGISTER || inst->Args[1].Type == AtomNode::AtomType::IDENTIFIER)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects char|uint|int|float as its 2nd argument.", inst->Line, inst->Character);
                        exit(2);
                    }
                }
                else if (fmt[1] == 9)
                {
                    if (inst->Args[1].Type != AtomNode::AtomType::IDENTIFIER)
                    {
                        Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects identifier as its 2nd argument.", inst->Line, inst->Character);
                        exit(2);
                    }
                    
                    if (type == OpCode::CALL)
                    {
                        if (!findInCodeSection(inst->Args[1].Str))
                        {
                            Log(LogLevel::ERR, "Invalid routine name `" + inst->Args[0].Str + "` in instruction `CALL`.", inst->Line, inst->Character);
                            exit(2);
                        }
                    }
                }
            }
            else
            {
                Log(LogLevel::ERR, "Instruction `" + inst->Inst + "` expects 2 arguments.", inst->Line, inst->Character);
                exit(2);
            }
        }

        void SemanticAnalyzer::analyzeRoutine()
        {
            for (auto & label : m_Routine->Labels)
            {
                for (auto & inst : label.Instructions)
                {
                    analyzeInstruction(&inst);
                }
            }
        }

        void SemanticAnalyzer::analyzeCodeSection()
        {
            for (auto & routine : m_Code->Routines)
            {
                m_Routine = &routine;
                analyzeRoutine();
            }
        }

        ASTNode * SemanticAnalyzer::analyze()
        {
            if (auto module = dynamic_cast<ModuleNode *>(m_Node)) { m_Code = &module->CodeSection; analyzeCodeSection(); }

            return m_Node;
        }
    }
}
