#ifndef FALCON_REGISTER_HPP
#define FALCON_REGISTER_HPP

#include <string>
#include <vector>

#include <cstdint>

namespace Falcon
{
    namespace RegisterType
    {
        enum RegisterType : uint8_t
        {
            R1, R2, R3, R4
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
