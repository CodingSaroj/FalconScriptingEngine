/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "Object.hpp"

namespace Falcon
{
    namespace FALI
    {
        std::unordered_map<std::string, Object::ObjectData> Object::s_ObjectTypes =
        {
            {"void",    {false, 0, {}}},
            
            {"char",    {false, 1, {{"base", {"char", 0}}}}},
            {"uchar",   {false, 1, {{"base", {"uchar", 0}}}}},
            {"short",   {false, 2, {{"base", {"short", 0}}}}},
            {"ushort",  {false, 2, {{"base", {"ushort", 0}}}}},
            {"int",     {false, 4, {{"base", {"int", 0}}}}},
            {"uint",    {false, 4, {{"base", {"uint", 0}}}}},
            {"long",    {false, 8, {{"base", {"long", 0}}}}},
            {"ulong",   {false, 8, {{"base", {"ulong", 0}}}}},

            {"float",   {false, 4, {{"base", {"float", 0}}}}},
            {"double",  {false, 8, {{"base", {"double", 0}}}}},

            {"ptr",     {true, 8, {{"base", {"ulong", 0}}}}},

            {"int8",    {true, 1, {{"base", {"char", 0}}}}},
            {"uint8",   {true, 1, {{"base", {"uchar", 0}}}}},
            {"int16",   {true, 2, {{"base", {"short", 0}}}}},
            {"uint16",  {true, 2, {{"base", {"ushort", 0}}}}},
            {"int32",   {true, 4, {{"base", {"int", 0}}}}},
            {"uint32",  {true, 4, {{"base", {"uint", 0}}}}},
            {"int64",   {true, 8, {{"base", {"long", 0}}}}},
            {"uint64",  {true, 8, {{"base", {"ulong", 0}}}}},

            {"float32", {true, 4, {{"base", {"float", 0}}}}},
            {"float64", {true, 8, {{"base", {"double", 0}}}}},
        };

        void Object::New(const std::string & name, bool alias)
        {
            s_ObjectTypes[name] = {alias};
        }

        void Object::AddMember(const std::string & objectType, const std::string & memberType, const std::string & memberName)
        {
            s_ObjectTypes[objectType].MemberOffsets[memberName] = { memberType, s_ObjectTypes[objectType].Size };
            s_ObjectTypes[objectType].Size += s_ObjectTypes[memberType].Size;
        }

        Object::Object(const std::string & type)
            : m_Temparory(false), m_ObjectType(type), m_ObjectData(&s_ObjectTypes[type]), m_Data((uint8_t *)ObjectSpace::New(s_ObjectTypes[type].Size))
        {
        }
        
        Object::Object(const std::string & type, void * rawObj)
            : m_Temparory(true), m_ObjectType(type), m_ObjectData(&s_ObjectTypes[type]), m_Data((uint8_t *)rawObj)
        {
        }
        
        Object::Object(const std::string & type, const std::string & constructor, std::vector<void *> args)
            : m_Temparory(false), m_ObjectType(type), m_ObjectData(&s_ObjectTypes[type]), m_Data((uint8_t *)ObjectSpace::New(s_ObjectTypes[type].Size))
        {
            memcpy(m_Data, Function::Get(constructor).Call(args), m_ObjectData->Size);
        }

        Object::~Object()
        {
            if (!m_Temparory)
            {
                ObjectSpace::Delete(m_Data);
            }
        }
        
        Object Object::CallStatic(const std::string & retType, const std::string & function, std::vector<void *> args)
        {
            std::vector<std::string> names;
            names.reserve(args.size());
            
            for (auto & arg : args)
            {
                names.emplace_back(((Object *)arg)->m_ObjectType);
                arg = ((Object *)arg)->Self();
            }

            Function & func = Function::Get(MangleFunction(function, names));
            
            return Object(retType, func.Call(args));
        }

        Object Object::CallMember(const std::string & retType, const std::string & function, std::vector<void *> args)
        {
            args.emplace(args.begin(), (void *)this);

            return CallStatic(retType, function, args);
        }
    }
}
