#ifndef FALCON_FALI_TYPES_HPP
#define FALCON_FALI_TYPES_HPP

#include "common/Common.hpp"

#include "fali/TypeFlags.hpp"

namespace Falcon
{
    namespace FALI
    {
        static std::unordered_map<std::string, size_t> s_Types
        {
            {"void", 0},
            {"bool", 1},
            {"uint8", 1},
            {"uint16", 2},
            {"uint32", 4},
            {"uint64", 8},
            {"int8", 1},
            {"int16", 2},
            {"int32", 4},
            {"int64", 8},
            {"float32", 4},
            {"float64", 8},
            {"ptr", 8}
        };

        struct DataType
        {
            std::string Name;
            TypeFlag    Flags;

            inline size_t GetSize()
            {
                if (Flags & TypeFlags::TEMPARORY || Flags & TypeFlags::REFERENCE)
                {
                    return 8;
                }
                else
                {
                    return s_Types[Name];
                }
            }

            std::string Str() const;
        };

        std::ostream & operator<<(std::ostream & out, const DataType & type);
    }
}

#endif
