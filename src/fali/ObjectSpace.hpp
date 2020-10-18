/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#ifndef FALCON_FALI_OBJECT_SPACE_HPP
#define FALCON_FALI_OBJECT_SPACE_HPP

#include "common/Common.hpp"

#ifndef FALI_OBJECT_SPACE_SIZE
#define FALI_OBJECT_SPACE_SIZE 65536
#endif

namespace Falcon
{
    namespace FALI
    {
        class ObjectSpace
        {
        public:
            static void * New(size_t size);
            static void Delete(void * ptr);
        
        private:
            static uint8_t s_Data[FALI_OBJECT_SPACE_SIZE];
            static std::vector<std::tuple<bool, void *, size_t>> s_Pointers;
        };
    }
}

#endif
