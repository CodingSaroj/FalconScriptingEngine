#ifndef FALCON_COMMON_ENDIAN_HPP
#define FALCON_COMMON_ENDIAN_HPP

#include <algorithm>
#include <array>

#include <cstdint>

namespace Falcon
{
    namespace Endian
    {
        enum class Endian
        {
            Little,
            Big
        };

        inline Endian GetSystemEndian()
        {
            uint32_t i = 1;

            return ((char *)&i)[0] ? Endian::Little : Endian::Big;
        }

        template <typename Type>
        void ByteSwap(Type & data)
        {
            auto & raw = *(std::array<uint8_t, sizeof(Type)> *)&data;

            std::reverse(raw.begin(), raw.end());
        }

        template <typename Type>
        Type SystemToLittle(Type value)
        {
            if (GetSystemEndian() == Endian::Big)
            {
                ByteSwap(value);
            }
            
            return value;
        }

        template <typename Type>
        Type LittleToSystem(Type value)
        {
            if (GetSystemEndian() == Endian::Big)
            {
                ByteSwap(value);
            }

            return value;
        }
    }
}

#endif
