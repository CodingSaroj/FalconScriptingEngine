#ifndef FALCON_VM_SIGNAL_HPP
#define FALCON_VM_SIGNAL_HPP

#include "common/Common.hpp"

#include "vm/VM.hpp"
#include "vm/Debugger.hpp"

namespace Falcon
{
    class Signal
    {
    public:
        static void SetTargetVM(VM * targetVM, bool debugger = false);

        static void Raise(int signal);

    private:
        static VM * s_TargetVM;

        static std::unordered_map<int, std::function<void(int)>> s_Handlers;
    };
}

#endif
