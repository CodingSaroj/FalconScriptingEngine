#ifndef FALCON_FALI_READER_HPP
#define FALCON_FALI_READER_HPP

#include "common/Common.hpp"
#include "common/Endian.hpp"

#include "vm/DebugData.hpp"

#include "fali/Attribute.hpp"
#include "fali/Object.hpp"

namespace Falcon
{
    namespace FALI
    {
        class Reader
        {
        public:
            Reader(const std::string & fileName);

            uint8_t * GetCode();
            DebugData GetDebugData();

        private:
            uint8_t * m_Code;
            DebugData m_DebugData;

            void ReadCodeSection(uint8_t * start, uint64_t size);
            void ReadDebugSection(uint8_t * start, uint64_t size);
            void ReadReflectionSection(uint8_t * start, uint64_t size);
        };
    }
}

#endif
