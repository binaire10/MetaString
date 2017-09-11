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

        constexpr operator const T_t *() const noexcept
        {
            return data;
        }

        template<T_t... str2>
        constexpr Content<T_t, str..., str2...> operator+(Content<T_t, str2...>) noexcept
        {
            return Content<T_t, str..., str2...>{};
        }
    };

    template<typename T_t, T_t... str>
    constexpr typename Content<T_t, str...>::res Content<T_t, str...>::data;

}

