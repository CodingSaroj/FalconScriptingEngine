#ifndef FALCON_FALI_ATTRIBUTE_HPP
#define FALCON_FALI_ATTRIBUTE_HPP

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

namespace Falcon
{
    namespace FALI
    {
        class AttributeTable
        {
        public:
            static void New(const std::string & name, std::vector<std::string> attribs);


            static std::vector<std::string> & GetAttributes(const std::string & name);
            static std::string GetFunctionWithAttribute(const std::string & attrib);

        private:
            static std::unordered_map<std::string, std::vector<std::string>> s_Attributes;
        };
    }
}

#endif
