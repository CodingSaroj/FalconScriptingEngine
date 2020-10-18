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

            {0, 4}, {0, 4}, {0, 4}, {0, 4},
            {0, 4},

            {4, 3}, {4, 3},

            {5, 3}, {5, 3}, {5, 3}, {4, 9},
            {9, 3}, {4, 3}, {3, 3}, {3, 3}
        };

        SemanticAnalyzer::SemanticAnalyzer(ASTNode * node)
            : m_Node(node)
        {
        }

        bool SemanticAnalyzer::FindInCodeSection(const std::string & name)
        {
            return std::find_if(m_Code->Routines.begin(), m_Code->Routines.end(), [&name](const RoutineNode & routine)->bool
            {
                return routine.Name == name;
            }) != m_Code->Routines.end();
        }
        
        bool SemanticAnalyzer::FindInRoutine(const std::string & name)
        {
            return std::find_if(m_Routine->Labels.begin(), m_Routine->Labels.end(), [&name](const LabelNode & label)->bool
            {
                return label.Name == name;
            }) != m_Routine->Labels.end();
        }

        void SemanticAnalyzer::AnalyzeInstruction(InstructionNode * inst)
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
                FLCN_REL_ASSERT(inst->Args.empty(), "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects 0 argument.", inst->Line, inst->Inst);
            }
            else if (inst->Args.size() != 0)
            {
                if (fmt[0] == 0)
                {
                    FLCN_REL_ASSERT(inst->Args[0].Register, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects register as its 1st argument.", inst->Line, inst->Inst);
                }
                else if (fmt[0] == 1 || fmt[0] == 2)
                {
                    FLCN_REL_ASSERT(inst->Args[0].Char || inst->Args[0].Uint || inst->Args[0].Int, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects char|uint|int as its 1st argument.", inst->Line, inst->Inst);
                }
                else if (fmt[0] == 4 || fmt[0] == 8)
                {
                    FLCN_REL_ASSERT(!inst->Args[0].Register || !inst->Args[0].Str, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects char|uint|int|float as its 1st argument.", inst->Line, inst->Inst);
                }
                else if (fmt[0] == 9)
                {
                    FLCN_REL_ASSERT(inst->Args[0].Str, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects identifier as its 1st argument.", inst->Line, inst->Inst);
                }
                else if (fmt[0] == 5)
                {
                    FLCN_REL_ASSERT(!inst->Args[0].Register, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects char|uint|int|float|identifier as its 1st argument.", inst->Line, inst->Inst);

                    if (inst->Args[0].Str)
                    {
                        FLCN_REL_ASSERT(FindInRoutine(*inst->Args[0].Str), "Assembler::SemanticAnalyzer", "{}: Label `{}` not found in routine `{}`.", inst->Line, *inst->Args[0].Str, m_Routine->Name);
                    }
                }
            }
            else
            {
                if (fmt[1] == 3)
                {
                    FLCN_REL_ASSERT(false, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects 1 arguments.", inst->Line, inst->Inst);
                }
                else
                {
                    FLCN_REL_ASSERT(false, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects 2 arguments.", inst->Line, inst->Inst);
                }
            }

            if (fmt[1] == 3)
            {
                if (fmt[0] == 3)
                {
                    FLCN_REL_ASSERT(inst->Args.size() == 0, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects 1 argument.", inst->Line, inst->Inst);
                }
                else
                {
                    FLCN_REL_ASSERT(inst->Args.size() == 1, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects 2 arguments.", inst->Line, inst->Inst);
                }
            }
            else if (inst->Args.size() == 2)
            {
                if (fmt[1] == 0)
                {
                    FLCN_REL_ASSERT(inst->Args[0].Register, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects register as its 2nd argument.", inst->Line, inst->Inst);
                }
                else if (fmt[1] == 1 || fmt[1] == 2)
                {
                    FLCN_REL_ASSERT(inst->Args[1].Char || inst->Args[1].Uint || inst->Args[1].Int, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects char|uint|int as its 2nd argument.", inst->Line, inst->Inst);
                }
                else if (fmt[1] == 4 || fmt[0] == 8)
                {
                    FLCN_REL_ASSERT(!inst->Args[1].Register || !inst->Args[1].Str, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects char|uint|int|float as its 2nd argument.", inst->Line, inst->Inst);
                }
                else if (fmt[0] == 9)
                {
                    FLCN_REL_ASSERT(inst->Args[0].Str, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects identifier as its 2nd argument.", inst->Line, inst->Inst);

                    if (type == OpCode::CALL)
                    {
                        FLCN_REL_ASSERT(FindInCodeSection(*inst->Args[0].Str), "Assembler::SemanticAnalyzer", "{}: Routine `{}` not found in code section.", inst->Line, *inst->Args[0].Str);
                    }
                }
            }
            else
            {
                FLCN_REL_ASSERT(false, "Assembler::SemanticAnalyzer", "{}: Instruction `{}` expects 2 arguments.", inst->Line, inst->Inst);
            }
        }

        void SemanticAnalyzer::AnalyzeRoutine()
        {
            for (auto & label : m_Routine->Labels)
            {
                for (auto & inst : label.Instructions)
                {
                    AnalyzeInstruction(&inst);
                }
            }
        }

        void SemanticAnalyzer::AnalyzeCodeSection()
        {
            for (auto & routine : m_Code->Routines)
            {
                m_Routine = &routine;
                AnalyzeRoutine();
            }
        }

        ASTNode * SemanticAnalyzer::Analyze()
        {
            if (auto module = dynamic_cast<ModuleNode *>(m_Node)) 
            {
                m_Code = &module->CodeSection;
                AnalyzeCodeSection();
            }

            return m_Node;
        }
    }
}
