#ifndef FALCON_VM_SIGNAL_IMPLEMENT_HPP
#define FALCON_VM_SIGNAL_IMPLEMENT_HPP

#include <csignal>
#include <climits>

#include "Signal.hpp"

namespace Falcon
{
    void ImplementSignals()
    {
        for (auto & sig : std::array<int, 6>({SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM}))
        {
            signal(sig, Signal::Raise);
        }
    }
}

#endif
