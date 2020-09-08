#ifndef FALCON_FALI_OBJECT_HPP
#define FALCON_FALI_OBJECT_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

#include <cstring>

#include "fali/ObjectSpace.hpp"
#include "fali/Mangle.hpp"
#include "fali/Function.hpp"

namespace Falcon
{
    namespace FALI
    {
        class Object
        {
        private:
            struct ObjectData
            {
                bool Alias;
                uint64_t Size;
                std::unordered_map<std::string, std::pair<std::string, uint64_t>> MemberOffsets;
            };

        public:
            using MemberIterator = std::unordered_map<std::string, std::pair<std::string, uint64_t>>::iterator;
            using ObjectIterator = std::unordered_map<std::string, ObjectData>::iterator;

        private:
            struct ObjectIterable
            {
                inline ObjectIterator begin()
                {
                    return s_ObjectTypes.begin();
                }
                
                inline ObjectIterator end()
                {
                    return s_ObjectTypes.end();
                }
            };

            class MemberIterable
            {
            private:
                std::string m_ObjectName;

            public:
                inline MemberIterable(const std::string & objectName)
                    : m_ObjectName(objectName)
                {
                }

                inline MemberIterator begin()
                {
                    return s_ObjectTypes[m_ObjectName].MemberOffsets.begin();
                }

                inline MemberIterator end()
                {
                    return s_ObjectTypes[m_ObjectName].MemberOffsets.end();
                }
            };

        public:
            Object(const std::string & type);
            Object(const std::string & type, void * rawObj);
            Object(const std::string & type, const std::string & constructor, std::vector<void *> args);
            ~Object();
            
            constexpr uint8_t * & self()
            {
                return m_Data;
            }
            
            Object callStatic(const std::string & retType, const std::string & function, std::vector<void *> args);
            Object callMember(const std::string & retType, const std::string & function, std::vector<void *> args);

            template <typename T>
            T & get(const std::string & member)
            {
                return *(T *)(m_Data + m_ObjectData->MemberOffsets[member].second);
            }

            template <typename T>
            T & safeGet(const std::string & member, const std::string & type)
            {
                if (m_ObjectData->MemberOffsets[member].first == type)
                {
                    return *(T *)(m_Data + m_ObjectData->MemberOffsets[member].second);
                }
                else
                {
                    std::cout<<"TypeMismatch: Variable `"<<member<<"` is of type `"<<m_ObjectData->MemberOffsets[member].first<<"` not `"<<type<<"`.\n";
                    exit(2);
                }
            }
            
            static void New(const std::string & name, bool alias = false);
            static void AddMember(const std::string & objectType, const std::string & memberType, const std::string & memberName);

            static inline bool           IsValid(const std::string & objectName) { return s_ObjectTypes.count(objectName) != 0; }

            static inline ObjectData     GetObjectData(const std::string & objectName) { return s_ObjectTypes[objectName]; }
            static inline ObjectIterable GetObjectIterable() { return ObjectIterable(); }
            static inline MemberIterable GetMemberIterable(const std::string & objectName) { return MemberIterable(objectName); }

        private:
            static std::unordered_map<std::string, ObjectData> s_ObjectTypes;

            bool              m_Temparory;
            const std::string m_ObjectType;
            ObjectData *      m_ObjectData;
            uint8_t *         m_Data;
        };

        template <typename ReturnType, typename... Args, uint32_t... Ints>
        Function CreateFunction(const std::string & retType, std::function<ReturnType(Args...)> function, std::integer_sequence<uint32_t, Ints...>)
        {
            return Function((Function::FunctionType)[=](std::array<void *, sizeof...(Args)> args)->void *
            {
                if constexpr (std::is_void_v<ReturnType>)
                {
                    function((*(std::remove_reference_t<Args> *)args[Ints])...);
        
                    return nullptr;
                }
                else
                {
                    return (void *)new Object(retType, (void *)function(*(Args *)args[0]...)); 
                }
            });
        }
        
        template <typename ReturnType, typename... Args>
        inline Function CreateFunction(const std::string & retType, std::function<ReturnType(Args...)> function)
        {
            return CreateFunction<ReturnType, Args...>(retType, function, std::make_integer_sequence<uint32_t, sizeof...(Args)>{});
        }
    }
}

#endif
