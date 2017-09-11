#pragma once

namespace meta
{
    template<typename T_t, T_t... str>
    struct Content
    {
        enum{
            Size = sizeof... (str)
        };
        typedef T_t (res)[Size+1];
        static constexpr const res &get()
        { return data; }
        template<typename Type_t>
        static constexpr Type_t agregat()
        { return Type_t{data}; }
        static constexpr res data = {str..., T_t(0)};
        template<typename Out, typename Function>
        static constexpr Out Apply(Function &f)
        {
            return f(str...);
        }
        constexpr operator const T_t *() const
        {
            return data;
        }
    };

    template<typename T_t, T_t... str>
    constexpr typename Content<T_t, str...>::res Content<T_t, str...>::data;
}
