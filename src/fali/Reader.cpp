#include "Reader.hpp"

namespace Falcon
{
    namespace FALI
    {
        Reader::Reader(const std::string & fileName)
        {
            std::ifstream in(fileName);

            char buffer[8];

            in.read(buffer, 4);

            if (std::string(buffer, 4) != "FLCN")
            {
                std::cout<<"File not falcon bytecode!";
                exit(2);
            }

            in.read(buffer, 8);

            m_Code = new uint8_t[*(uint64_t *)buffer];

            in.read((char *)m_Code, *(uint64_t *)buffer);

            in.close();
        }

        uint8_t * Reader::GetCode()
        {
            return m_Code;
        }
    }
}
