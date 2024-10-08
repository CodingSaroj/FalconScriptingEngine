/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_VM_DEBUGGER_HPP
#define FALCON_VM_DEBUGGER_HPP

#include "common/Common.hpp"
#include "common/CLIColors.hpp"

#include "vm/DebugData.hpp"
#include "vm/VM.hpp"

#include "vm/Disassembler.hpp"

namespace Falcon
{
    class Debugger : public VM
    {
    public:
        using FunctionMangleFunction = std::string( * )(const std::string &, std::vector<std::string>);
        using FunctionDemangleFunction = std::string( * )(const std::string &);
        using IdentifierMangleFunction = std::string( * )(const std::string &);
        using IdentifierDemangleFunction = std::string( * )(const std::string &);
        using PrintVarFunction = std::string( * )(const std::string &, void *);

        struct DebuggerFunctions
        {
            FunctionMangleFunction FunctionMangleFn;
            FunctionDemangleFunction FunctionDemangleFn;

            IdentifierMangleFunction IdentifierMangleFn;
            IdentifierDemangleFunction IdentifierDemangleFn;
        };

        Debugger(uint8_t * code, const DebugData & debugData, const DebuggerFunctions & functions, const std::string & debuggerName = "", const PrintVarFunction & printVarFunction = nullptr);

        void SetBreakpoint(uint64_t ip);
        void ClearBreakpoint(uint64_t ip);

        void Shell();

        std::unordered_map<int, std::function<void(int)>> GetSignalHandlers();

        void Run(const std::string & function, uint64_t argsSize = 0);

    private:
        DebugData m_DebugData;

        bool m_Next;
        bool m_NextI;
        bool m_Continue;
        bool m_Finish;

        uint64_t m_IC;
        uint64_t m_LC;

        uint32_t m_LastReturnSize;

        std::string m_CurrentLine;
        std::string m_CurrentFunction;

        std::vector<std::string> m_StackTrace;

        std::map<uint64_t, std::string> m_Disassembly;

        std::vector<uint64_t> m_Breakpoints;

        std::string m_DebuggerName;

        DebuggerFunctions m_DebuggerFunctions;
        PrintVarFunction m_PrintVarFunction;

        void Disassemble();

        void UpdateDebuggerState();
    };
}

#endif
