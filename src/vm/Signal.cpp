#include "Signal.hpp"

namespace Falcon
{
    VM * Signal::s_TargetVM = nullptr;
    std::unordered_map<int, std::function<void(int)>> Signal::s_Handlers;

    void Signal::SetTargetVM(VM * targetVM, bool debugger)
    {
        s_TargetVM = targetVM;

        if (s_TargetVM)
        {
            if (debugger)
            {
                s_Handlers = std::move(((Debugger *)s_TargetVM)->GetSignalHandlers());
            }
            else
            {
                s_Handlers = std::move(s_TargetVM->GetSignalHandlers());
            }
        } 
    }

    void Signal::Raise(int signal)
    {
        if (!s_TargetVM)
        {
            return;
        }

        if (s_TargetVM->IsRunning() && s_Handlers.count(signal))
        {
            s_Handlers[signal](signal);
        }
    }
}
