#ifndef FALCON_FALI_OBJECT_HPP
#define FALCON_FALI_OBJECT_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "Function.hpp"

namespace Falcon
{
    namespace FALI
    {
        class Object
        {
        private:
            struct ObjectData
            {
                uint64_t Size;
                std::unordered_map<std::string, Function> Functions;
                std::unordered_map<std::string, uint64_t> MemberOffsets;
            };

        public:
            static void New(const std::string & name, ObjectData data);

            Object(const std::string & type);
            ~Object();

            template <typename... Args>
            Object(const std::string & type, const std::string & constructor, Args... cargs)
                : m_ObjectType(type), m_Data(new uint8_t[s_ObjectTypes[type].Size])
            {
                s_ObjectTypes[m_ObjectType].Functions[constructor].call<void, uint8_t *, Args...>(self(), std::forward<Args>(cargs)...);
            }

            constexpr uint8_t * self()
            {
                return m_Data;
            }

            template <typename ReturnType, typename... Args>
            ReturnType call(const std::string & function, Args... args)
            {
                Function & func = s_ObjectTypes[m_ObjectType].Functions[function];

                if constexpr (!std::is_void_v<ReturnType>)
                {
                    return func.call<ReturnType, Args...>(args...);
                }

                func.call<ReturnType, Args...>(args...);
            }

            template <typename ReturnType, typename... Args>
            ReturnType callMember(const std::string & function, Args... args)
            {
                if constexpr (!std::is_void_v<ReturnType>)
                {
                    return call<ReturnType, uint8_t *, Args...>(function, self(), args...);
                }
                
                call<ReturnType, uint8_t *, Args...>(function, self(), args...);
            }

            template <typename T>
            T & get(const std::string & member)
            {
                return *(T *)(m_Data + s_ObjectTypes[m_ObjectType].MemberOffsets[member]);
            }

        private:
            static std::unordered_map<std::string, ObjectData> s_ObjectTypes;

            const std::string m_ObjectType;
            uint8_t *         m_Data;
        };
    }
}

#endif
