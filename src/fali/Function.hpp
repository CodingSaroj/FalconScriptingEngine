/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_FALI_FUNCTION_HPP
#define FALCON_FALI_FUNCTION_HPP

#include "common/Common.hpp"

namespace Falcon
{
    namespace FALI
    {
        class Function
        {
        public:
            struct FunctionData
            {
                std::string              ReturnType;
                std::string              Name;
                std::vector<std::string> ParameterTypes;

                Function * Base;
            };

            using FunctionIterator = std::unordered_map<std::string, FunctionData>::iterator;

        private:
            struct Iterable
            {
                inline FunctionIterator begin()
                {
                    return s_Functions.begin();
                }

                inline FunctionIterator end()
                {
                    return s_Functions.end();
                }
            };

        public:
            using FunctionType = std::function<void *(std::vector<void *>)>;

            Function() = default;

            explicit Function(FunctionType function);

            void * Call(std::vector<void *> args);
            
            static void AddFunction(const std::string & retType, const std::string & name, std::vector<std::string> paramTypes);
            static void AddFunction(const std::string & retType, const std::string & name, std::vector<std::string> paramTypes, Function base);
            
            static void SetBase(const std::string & name, Function base);

            static inline bool IsValid(const std::string & name) { return s_Functions.count(name) != 0; }

            static inline Function & Get(const std::string & name) { return *s_Functions[name].Base; }
            static inline Iterable GetIterable() { return Iterable(); }

        private:
            static std::unordered_map<std::string, FunctionData> s_Functions;

            FunctionType m_Function;

            friend class Context;
        };
    }
}

#endif
