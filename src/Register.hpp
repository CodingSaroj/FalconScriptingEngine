#ifndef FALCON_REGISTER_HPP
#define FALCON_REGISTER_HPP

#include <cstdint>

namespace Falcon
{
    namespace RegisterType
    {
        enum RegisterType : uint8_t
        {
            A8, A16, A32, A64,
            B8, B16, B32, B64,
            C8, C16, C32, C64,
            D8, D16, D32, D64,
            R1, R2,  R3,  R4,
            SP, CMT, AO0, AO1,
        };
    }

    union Register
    {
        uint8_t  u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;

        void *   ptr;
    };
}

#endif
