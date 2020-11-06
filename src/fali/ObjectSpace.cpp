/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "ObjectSpace.hpp"

namespace Falcon
{
    namespace FALI
    {
        uint8_t                                       ObjectSpace::s_Data[FALI_OBJECT_SPACE_SIZE];
        std::vector<std::tuple<bool, void *, size_t>> ObjectSpace::s_Pointers;

        void * ObjectSpace::New(size_t size)
        {
            if (size < FALI_OBJECT_SPACE_SIZE)
            {
                s_Pointers.emplace_back(false, new(s_Data) uint8_t[size], size);
            }
            else
            {
                s_Pointers.emplace_back(true, new uint8_t[size], size);
            }

            return std::get<1>(s_Pointers.back());
        }

        void ObjectSpace::Delete(void * ptr)
        {
            auto iter = std::find_if(s_Pointers.begin(), s_Pointers.end(), [&](std::tuple<bool, void *, size_t> & v1)->bool{ return std::get<1>(v1) == ptr; });

            if (iter != s_Pointers.end())
            {
                if (!std::get<0>(*iter))
                {
                    for (size_t i = 0; i < std::get<2>(*iter); i++)
                    {
                        ((uint8_t *)std::get<1>(*iter) + i)->~uint8_t();
                    }
                }
                else
                {
                    delete[] (uint8_t *)ptr;
                }
            }
        }
    }
}
