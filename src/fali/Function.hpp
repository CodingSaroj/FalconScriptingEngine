#ifndef FALCON_FALI_FUNCTION_HPP
#define FALCON_FALI_FUNCTION_HPP

#include <functional>

namespace Falcon
{
    namespace FALI
    {
        class Function
        {
            public:
                Function() = default;

                template <typename ReturnType, typename... Args>
                Function(ReturnType( * function)(Args...))
                    : m_Function((void *)function)
                {
                }

                template <typename ReturnType, typename... Args>
                ReturnType call(Args... args)
                {
                    auto function = (ReturnType( * )(Args...))m_Function;

                    if constexpr (!std::is_void_v<ReturnType>)
                    {
                        return function(std::forward<Args>(args)...);
                    }
                    
                    function(std::forward<Args>(args)...);
                }

            private:
                void * m_Function;
        };
    }
}

#endif
