#include "Object.hpp"

namespace Falcon
{
    namespace FALI
    {
        std::unordered_map<std::string, Object::ObjectData> Object::s_ObjectTypes =
        {
            {"char",    {1, {}, {{"base", 0}}}},
            {"short",   {2, {}, {{"base", 0}}}},
            {"int",     {4, {}, {{"base", 0}}}},
            {"long",    {8, {}, {{"base", 0}}}},
            {"float",   {4, {}, {{"base", 0}}}},
            {"double",  {8, {}, {{"base", 0}}}}
        };

        void Object::New(const std::string & name, ObjectData data)
        {
            s_ObjectTypes[name] = data;
        }

        Object::Object(const std::string & type)
            : m_ObjectType(type), m_Data(new uint8_t[s_ObjectTypes[type].Size])
        {
        }

        Object::~Object()
        {
            delete[] m_Data;
        }
    }
}
