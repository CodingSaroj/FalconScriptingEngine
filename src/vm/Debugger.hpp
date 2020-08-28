#ifndef FALCON_VM_DEBUGGER_HPP
#define FALCON_VM_DEBUGGER_HPP

#include <iostream>
#include <iomanip>
#include <map>

#include "DebuggerData.hpp"
#include "VM.hpp"

namespace Falcon
{
    class Debugger : public VM
    {
    public:
        using PrintVarFunction = std::string( * )(const std::string &, void *);

        Debugger(uint8_t * code, const DebuggerData & debugData, PrintVarFunction printVarFunction = nullptr);

        void setBreakpoint(uint64_t ip);
        void clearBreakpoint(uint64_t ip);

        void run(const std::string & function = "main", uint64_t argsSize = 0);

    private:
        DebuggerData m_DebugData;

        bool m_Next;
        bool m_Continue;
        bool m_Finish;

        uint64_t m_IC;
        uint64_t m_LC;

        std::string m_CurrentLine;
        std::string m_CurrentFunction;

        std::map<uint64_t, std::string> m_Disassembly;

        std::vector<uint64_t> m_Breakpoints;

        PrintVarFunction m_PrintVarFunction;

        void disassemble();
        std::string disassembleRegister(uint8_t reg);

        void updateDebuggerState();

        void shell();
    };
}

#endif
