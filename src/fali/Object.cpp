#include "Object.hpp"

namespace Falcon
{
    namespace FALI
    {
        std::unordered_map<std::string, Object::ObjectData> Object::s_ObjectTypes =
        {
            {"void",    {0, {}}},
            {"char",    {1, {{"base", {"char", 0}}}}},
            {"short",   {2, {{"base", {"short", 0}}}}},
            {"int",     {4, {{"base", {"int", 0}}}}},
            {"long",    {8, {{"base", {"long", 0}}}}},
            {"float",   {4, {{"base", {"float", 0}}}}},
            {"double",  {8, {{"base", {"double", 0}}}}}
        };

        void Object::New(const std::string & name)
        {
            s_ObjectTypes[name] = {};
        }

        void Object::AddMember(const std::string & objectType, const std::string & memberType, const std::string & memberName)
        {
            s_ObjectTypes[objectType].MemberOffsets[memberName] = { memberType, s_ObjectTypes[objectType].Size };
            s_ObjectTypes[objectType].Size += s_ObjectTypes[memberType].Size;
        }

        Object::Object(const std::string & type)
            : m_Temparory(true), m_ObjectType(type), m_ObjectData(&s_ObjectTypes[type]), m_Data(new uint8_t[s_ObjectTypes[type].Size])
        {
        }
        
        Object::Object(const std::string & type, void * rawObj)
            : m_Temparory(true), m_ObjectType(type), m_ObjectData(&s_ObjectTypes[type]), m_Data((uint8_t *)rawObj)
        {
        }
        
        Object::Object(const std::string & type, const std::string & constructor, std::vector<void *> args)
            : m_ObjectType(type), m_ObjectData(&s_ObjectTypes[type]), m_Data(new uint8_t[s_ObjectTypes[type].Size])
        {
            memcpy(m_Data, Function::Get(constructor).call(args), m_ObjectData->Size);
        }

        Object::~Object()
        {
            if (!m_Temparory)
            {
                delete[] m_Data;
            }
        }
        
        Object Object::callStatic(const std::string & retType, const std::string & function, std::vector<void *> args)
        {
            std::vector<std::string> names;
            names.reserve(args.size());
            
            for (auto & arg : args)
            {
                names.emplace_back(((Object *)arg)->m_ObjectType);
                arg = ((Object *)arg)->self();
            }

            Function & func = Function::Get(MangleFunction(function, names));
            
            return Object(retType, func.call(args));
        }

        Object Object::callMember(const std::string & retType, const std::string & function, std::vector<void *> args)
        {
            args.emplace(args.begin(), (void *)this);

            return callStatic(retType, function, args);
        }
    }
}
