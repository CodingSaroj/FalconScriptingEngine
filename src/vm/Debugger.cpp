#include "Debugger.hpp"

namespace Falcon
{
    Debugger::Debugger(uint8_t * code, const DebugData & debugData, DebuggerFunctions functions, const std::string & debuggerName, PrintVarFunction printVarFunction)
        : VM(code), m_DebugData(debugData), m_Next(false), m_NextI(false), m_Continue(false), m_Finish(false), m_IC(0), m_LastReturnSize(0),
          m_DebuggerFunctions(functions), m_DebuggerName(debuggerName), m_PrintVarFunction(printVarFunction)
    {
        Disassemble();
    }

    void Debugger::Disassemble()
    {
        uint64_t ip = 0;

        while (m_Code[ip] != 255)
        {
            uint64_t currentIP = ip;

            m_Disassembly[currentIP] = Disassembler::DisassembleInstruction(&m_Code[ip], &ip);

            ip++;
        }
    }

    void Debugger::UpdateDebuggerState()
    {
        uint64_t i = 0;

        for (auto & inst : m_Disassembly)
        {
            if (inst.first == m_IP)
            {
                m_IC = i;
                break;
            }

            i++;
        }

        for (auto iter = m_DebugData.LineData.begin(); iter != m_DebugData.LineData.end(); iter++)
        {
            if (iter->first > m_IC)
            {
                iter--;

                m_LC = iter->second.first;
                m_CurrentLine = iter->second.second;
                break;
            }
            else if (iter->first == m_IC)
            {
                m_LC = iter->second.first;
                m_CurrentLine = iter->second.second;
                break;
            }
        }

        for (auto & function : m_DebugData.FunctionData)
        {
            if (m_LC >= function.second.StartLine && m_LC <= function.second.EndLine)
            {
                m_CurrentFunction = function.first;
            }
        }
    }

    void Debugger::SetBreakpoint(uint64_t pos)
    {
        if (std::find(m_Breakpoints.begin(), m_Breakpoints.end(), pos) == m_Breakpoints.end())
        {
            m_Breakpoints.emplace_back(pos);
        }
        else
        {
            std::cout<<"Breakpoint "<<m_Breakpoints.size()<<" already at Instruction Counter: "<<std::hex<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<pos<<std::dec<<Common::Colors::White<<".\n";
        }

        std::cout<<"Breakpoint "<<m_Breakpoints.size()<<" set at Instruction Counter: "<<std::hex<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<pos<<std::dec<<Common::Colors::White<<".\n";
    }

    void Debugger::ClearBreakpoint(uint64_t pos)
    {
        auto iter = m_Breakpoints.end();

        if ((iter = std::find(m_Breakpoints.begin(), m_Breakpoints.end(), pos)) != m_Breakpoints.end())
        {
            m_Breakpoints.erase(iter);
            std::cout<<"Cleared breakpoint "<<m_Breakpoints.size()<<" at Instruction Counter: "<<std::hex<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<pos<<std::dec<<Common::Colors::White<<".\n";
        }
        else
        {
            std::cout<<"No breakpoint set at Instruction Counter: "<<std::hex<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<pos<<std::dec<<Common::Colors::White<<".\n";
        }
    }
    
    void Debugger::Shell()
    {
        while (true)
        {
            std::cout<<m_DebuggerName<<"> ";

            std::string cmd;

            std::cin>>cmd;

            if (cmd == "r" || cmd == "run")
            {
                uint32_t argc;
                std::vector<std::string> argv;
                
                std::cin>>argc;

                argv.reserve(argc);

                for (uint32_t i = 0; i < argc; i++)
                {
                    std::string str;

                    std::cin>>str;

                    argv.emplace_back(std::move(str));
                }

                uint8_t * rawArgv = (uint8_t *)argv.data();

                Push((uint8_t *)&argc, 4);
                Push((uint8_t *)&rawArgv, 8);

                Run(m_DebuggerFunctions.FunctionMangleFn("main", {"uint32", "ptr"}), 12);
            }
            else if (cmd == "n" || cmd == "next")
            {
                m_Next = true;
                return;
            }
            else if (cmd == "ni" || cmd == "nexti")
            {
                m_NextI = true;
                return;
            }
            else if (cmd == "c" || cmd == "continue")
            {
                m_Continue = true;
                return;
            }
            else if (cmd == "f" || cmd == "finish")
            {
                m_Finish = true;
                return;
            }
            else if (cmd == "q" || cmd == "quit")
            {
                m_Running = false;
                return;
            }
            else if (cmd == "b" || cmd == "breakpoint")
            {
                uint64_t location;

                std::cin>>location;

                SetBreakpoint(location);
            }
            else if (cmd == "cl" || cmd == "clear")
            {
                uint64_t location;

                std::cin>>location;

                ClearBreakpoint(location);
            }
            else if (cmd == "reg" || cmd == "registers")
            {
                for (uint8_t i = 0; i < RegisterType::s_Names.size(); i++)
                {
                    Register & reg = GetRegister((RegisterType::RegisterType)i);

                    std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<RegisterType::s_Names[i]<<":"<<Common::Colors::White<<"\n";
                    std::cout<<"    u8:  "<<+reg.u8<<"\tu16: "<<reg.u16<<"\tu32: "<<reg.u32<<"\tu64: "<<reg.u64<<"\n\n";
                    std::cout<<"    i8:  "<<+reg.i8<<"\ti16: "<<reg.i16<<"\ti32: "<<reg.i32<<"\ti64: "<<reg.i64<<"\n\n";
                    std::cout<<"    f32: "<<reg.f32<<"\tf64: "<<reg.f64<<"\n";
                }
            }
            else if (cmd == "stk" || cmd == "stack")
            {
                uint64_t size;

                std::cin>>size;

                uint64_t lineCount = size / 16;

                std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"Stacktrace:\n";
                
                uint64_t offset = m_FP;

                for (int i = 0; i < lineCount; i++)
                {
                    std::cout<<"    "<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<std::hex<<std::setfill('0')<<std::setw(6)<<offset<<std::dec<<std::setw(0)<<Common::Colors::White<<" ";

                    offset += 16;
                    
                    for (int j = 0; j < 16; j++)
                    {
                        std::cout<<"0x"<<std::hex<<std::setfill('0')<<std::setw(2)<<+(m_Stack[m_FP + i * 16 + j])<<std::dec<<std::setw(0)<<(j < 15 ? " " : "\n");
                    }
                }

                if (size % 16)
                {
                    uint64_t rem = size % 16;

                    std::cout<<"    "<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<std::hex<<std::setfill('0')<<std::setw(6)<<offset<<std::dec<<std::setw(0)<<Common::Colors::White<<" ";

                    for (int i = 0; i < rem; i++)
                    {
                        std::cout<<"0x"<<std::hex<<std::setfill('0')<<std::setw(2)<<+(m_Stack[m_FP + lineCount * 16 + i])<<std::dec<<std::setw(0)<<(i < rem - 1 ? " " : "\n");
                    }
                }
            }
            else if (cmd == "stt" || cmd == "state")
            {
                std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"IC: "<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<std::hex<<m_IC<<Common::Colors::White<<" "<<std::dec<<m_IC<<".\n";
                std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"IP: "<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<std::hex<<m_IP<<Common::Colors::White<<" "<<std::dec<<m_IP<<".\n";
                std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"SP: "<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<std::hex<<m_SP<<Common::Colors::White<<" "<<std::dec<<m_SP<<".\n";
                std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"FP: "<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<std::hex<<m_FP<<Common::Colors::White<<" "<<std::dec<<m_FP<<".\n";
                std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"FLAGS: "<<Common::Colors::White<<"{ "<<(m_Cmp[0] ? "true" : "false")<<", "<<(m_Cmp[1] ? "true" : "false")<<" }.\n";
            }
            else if (cmd == "t" || cmd == "trace")
            {
                std::cout<<"Trace:\n";

                size_t depth = 0;

                for (auto iter = m_StackTrace.rbegin(); iter != m_StackTrace.rend(); iter++)
                {
                    std::cout<<"    #"<<depth++<<": "<<*iter<<"\n";
                }
            }
            else if (cmd == "dsasm" || cmd == "disassembly")
            {
                std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"Disassembly:"<<Common::Colors::White<<"\n";

                auto tmp = m_Disassembly.emplace(m_IP, "").first;

                uint64_t begin, end;

                for (auto iter = m_Disassembly.find(m_IP); true; iter--)
                {
                    if (iter->second == "func")
                    {
                        begin = iter->first;
                        break;
                    }

                    if (iter == m_Disassembly.begin())
                    {
                        break;
                    }
                }

                for (auto iter = m_Disassembly.find(m_IP); iter != m_Disassembly.end(); iter++)
                {
                    if (iter->second == "func")
                    {
                        end = iter->first;
                        break;
                    }
                }

                if (tmp->second == "")
                {
                    m_Disassembly.erase(tmp);
                }

                for (auto iter = m_Disassembly.find(begin); iter != m_Disassembly.find(end); iter++)
                {
                    if (iter->second == "func")
                    {
                        for (auto & f : m_Functions)
                        {
                            if (f.second == iter->first)
                            {
                                std::cout<<"    "<<Common::Colors::Green<<"["<<f.first<<"]"<<Common::Colors::White<<"\n";
                            }
                        }
                    }

                    if (iter->first == m_IP)
                    {
                        std::cout<<" "<<Common::Colors::Red<<">>"<<Common::Colors::White<<" ";
                    }
                    else
                    {
                        std::cout<<"    ";
                    }

                    uint64_t ic = 0;

                    for (auto & inst : m_Disassembly)
                    {
                        if (inst.first == iter->first)
                        {
                            break;
                        }

                        ic++;
                    }

                    std::cout<<std::setfill(' ')<<"+["<<std::setw(4)<<ic<<std::setw(0)<<"] +["<<std::setw(4)<<iter->first<<std::setw(0)<<"]\t\t"<<iter->second<<"\n";
                }
            }
            else if (cmd == "p" || cmd == "print")
            {
                std::string var;

                std::cin>>var;

                auto & data = m_DebugData.FunctionData[m_CurrentFunction];

                if (data.LocalVariables.count(var) == 1)
                {
                    if (data.LocalVariables[var].second < m_SP - m_FP)
                    {
                        if (m_PrintVarFunction)
                        {
                            std::cout<<data.LocalVariables[var].first<<" '"<<var<<"' = "<<m_PrintVarFunction(data.LocalVariables[var].first, (void *)&m_Stack[m_FP + data.LocalVariables[var].second])<<"\n";
                        }
                        else
                        {
                            std::cout<<"No `PrintVarFunction` defined.\n";
                        }
                    }
                    else
                    {
                        std::cout<<"Variable '"<<var<<"' is not defined.\n";
                    }
                }
                else
                {
                    std::cout<<"Variable '"<<var<<"' is not defined.\n";
                }
            }
        }
    }

    std::unordered_map<int, std::function<void(int)>> Debugger::GetSignalHandlers()
    {
        return
        {
            {
                SIGABRT,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Aborted.\n";
                    Shell();
                    exit(SIGABRT);
                }
            },
            {
                SIGILL,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Illegal CPU Instruction.\n";
                    Shell();
                    exit(SIGILL);
                }
            },
            {
                SIGINT,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Interrupted.\n";
                    Shell();
                    m_Running = false;
                }
            },
            {
                SIGFPE,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Floating point exception.\n";
                    Shell();
                    exit(SIGFPE);
                }
            },
            {
                SIGSEGV,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Segmentation fault.\n";
                    Shell();
                    exit(SIGSEGV);
                }
            },
            {
                SIGTERM,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Terminating.\n";
                    Shell();
                    m_Running = false;
                }
            }
        };
    }

    void Debugger::Run(const std::string & function, uint64_t argsSize)
    {
        m_StackTrace.emplace_back("base");
        m_StackTrace.emplace_back(m_DebugData.FunctionData[function].Signature);

        m_IP = m_Functions[function] + 1;

        uint8_t * args = new uint8_t[argsSize];

        memcpy(args, Pop(argsSize), argsSize);

        uint64_t returnIP = 0;

        Push((uint8_t *)&m_SP, 8);
        Push((uint8_t *)&returnIP, 8);
        Push((uint8_t *)&m_FP, 8);

        m_FP = m_SP;

        Push(args, argsSize);

        delete[] args;

        UpdateDebuggerState();

        m_Running = true;
        OpCode::OpCode op = (OpCode::OpCode)m_Code[m_IP];

        while (m_Running)
        {
            if (std::find(m_Breakpoints.begin(), m_Breakpoints.end(), m_IC) != m_Breakpoints.end())
            {
                m_Continue = false;
                std::cout<<"Breakpoint hit at Instruction Counter "<<(Common::Colors::Blue | Common::Colors::Bold)<<"0x"<<std::hex<<m_IC<<std::dec<<Common::Colors::White<<":\n";
                std::cout<<"    "<<(Common::Colors::Yellow | Common::Colors::Bold)<<"Line "<<m_LC<<": "<<Common::Colors::White<<m_CurrentLine<<"\n";
                Shell();

                if (!m_Running)
                {
                    break;
                }
            }

            if (m_Next)
            {
                m_Next = false;
                
                uint64_t currentLC = m_LC;

                while (m_LC == currentLC && m_Running)
                {
                    if (op == OpCode::CALL)
                    {
                        std::string functionName((char *)&m_Code[m_IP + 5]);

                        m_StackTrace.emplace_back(m_DebugData.FunctionData[functionName].Signature);
                    }
                    else if (op == OpCode::RET)
                    {
                        m_StackTrace.pop_back();

                        m_LastReturnSize = *(uint32_t *)&m_Code[m_IP + 1];
                    }

                    (this->*m_Operators[(uint8_t)op])();
                    
                    op = (OpCode::OpCode)m_Code[++m_IP];

                    UpdateDebuggerState();
                }

                std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"Line "<<m_LC<<": "<<Common::Colors::White<<m_CurrentLine<<"\n";

                Shell();
                
                if (!m_Running)
                {
                    break;
                }
            }
            
            if (op == OpCode::CALL)
            {
                std::string functionName((char *)&m_Code[m_IP + 5]);

                m_StackTrace.emplace_back(m_DebugData.FunctionData[functionName].Signature);
            }
            else if (op == OpCode::RET)
            {
                m_StackTrace.pop_back();

                m_LastReturnSize = *(uint32_t *)&m_Code[m_IP + 1];
            }

            (this->*m_Operators[(uint8_t)op])();

            if (m_Finish)
            {
                if (op == OpCode::RET)
                {
                    m_Finish = false;

                    std::cout<<"Finished `"<<m_DebugData.FunctionData[m_CurrentFunction].Signature<<"`.\n";

                    std::string returnType(m_DebugData.FunctionData[m_CurrentFunction].Signature.substr(0, m_DebugData.FunctionData[m_CurrentFunction].Signature.find_first_of(' ')));

                    std::cout<<"Value returned = "<<m_PrintVarFunction(returnType, &m_Stack[m_SP - m_LastReturnSize])<<"\n";

                    Shell();

                    if (!m_Running)
                    {
                        break;
                    }
                }
            }

            op = (OpCode::OpCode)m_Code[++m_IP];
            
            UpdateDebuggerState();
            
            if (m_NextI)
            {
                m_NextI = false;

                Shell();
            }
        }

        m_StackTrace.clear();
    }
}
