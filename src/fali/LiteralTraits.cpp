#include "LiteralTraits.hpp"

namespace Falcon
{
    namespace FALI
    {
        bool IsWholeLiteral(const std::string & varName)
        {
            bool isWhole = true;

            for (char c : varName)
            {
                if (c < '0' && c > '9')
                {
                    isWhole = false;
                    break;
                }
            }

            return isWhole;
        }

        bool IsIntegerLiteral(const std::string & varName)
        {
            if (varName[0] == '+' || varName[0] == '-')
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool IsRealLiteral(const std::string & varName)
        {
            if (varName.find('.') == std::string::npos)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
