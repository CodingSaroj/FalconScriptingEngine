#ifndef FALCON_REGISTER_HPP
#define FALCON_REGISTER_HPP

#include "common/Common.hpp"

namespace Falcon
{
    namespace RegisterType
    {
        enum RegisterType : uint8_t
        {
            R1,  R2,  R3,  R4,
            R5,  R6,  R7,  R8,
            R9,  R10, R11, R12,
            R13, R14, R15, R16
        };

        extern std::vector<std::string> s_Names;
   }

    union Register
    {
        uint8_t  u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;

        int8_t   i8;
        int16_t  i16;
        int32_t  i32;
        int64_t  i64;

        float    f32;
        double   f64;
    };
}

#endif
