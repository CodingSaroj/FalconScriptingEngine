/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_VM_SIGNAL_IMPLEMENT_HPP
#define FALCON_VM_SIGNAL_IMPLEMENT_HPP

#include "common/Common.hpp"

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
