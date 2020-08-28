#ifndef FALCON_FALI_READER_HPP
#define FALCON_FALI_READER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cstring>

namespace Falcon
{
    namespace FALI
    {
        class Reader
        {
        public:
            Reader(const std::string & fileName);

            uint8_t * GetCode();

        private:
            uint8_t * m_Code;
        };
    }
}

#endif
