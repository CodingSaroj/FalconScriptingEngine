#include "Types.hpp"

namespace Falcon
{
    namespace FALI
    {
        std::string DataType::Str() const
        {
            std::string str;

            if (Flags & TypeFlags::CONST)
            {
                str += "const ";
            }
            
            if (Flags & TypeFlags::TEMPARORY)
            {
                str += "tmp ";
            }

            str += Name;

            if (Flags & TypeFlags::REFERENCE)
            {
                str += " ref";
            }
            
            return std::move(str);
        }

        std::ostream & operator<<(std::ostream & out, const DataType & type)
        {
            out<<type.Str();

            return out;
        }
    }
}
