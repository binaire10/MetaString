#pragma once

#include "Content.hpp"

namespace meta
{
    template<typename T_t, size_t s, const T_t (&lstr)[s], size_t current=s, T_t... str>
    struct unserialize_meta : unserialize_meta<T_t, s, lstr, current-1, str..., lstr[s-current]>
    {};

    template<typename T_t, size_t s, const T_t (&lstr)[s], T_t... str>
    struct unserialize_meta<T_t, s, lstr, 1, str...>
    {
        typedef Content<T_t, str...> type;
    };

    template<typename...>
    struct serialize_meta;

    template<template<typename T_t, T_t... str> class Content_t, typename T_t, T_t... t>
    struct serialize_meta<Content_t<T_t,t...> >
    {
        typedef Content_t<T_t, t...> type;
    };

    template<template<typename T_t, T_t... str> class Content_t, typename T_t, T_t... t1, T_t... t2, typename... etc_t>
    struct serialize_meta<Content_t<T_t,t1...>, Content_t<T_t,t2...>, etc_t...> : serialize_meta<Content_t<T_t, t1..., t2...>, etc_t...>
    {};

    template<template<typename T_t, T_t... str> class Content_t, typename T_t, T_t... t1, T_t... t2, T_t... t3, T_t... t4, typename... etc_t>
    struct serialize_meta<Content_t<T_t,t1...>, Content_t<T_t,t2...>, Content_t<T_t,t3...>, Content_t<T_t,t4...>, etc_t...> : serialize_meta<Content_t<T_t, t1..., t2..., t3..., t4...>, etc_t...>
    {};

    template<template<typename T_t, T_t... str> class Content_t, typename T_t, T_t... t1, T_t... t2, T_t... t3, T_t... t4, T_t... t5, T_t... t6, T_t... t7, T_t... t8, typename... etc>
    struct serialize_meta<Content_t<T_t,t1...>, Content_t<T_t,t2...>, Content_t<T_t,t3...>, Content_t<T_t, t4...>, Content_t<T_t, t5...>, Content_t<T_t, t6...>, Content_t<T_t, t7...>, Content_t<T_t, t8...>, etc...> : serialize_meta<Content_t<T_t, t1..., t2..., t3..., t4..., t5..., t6..., t7..., t8...>, etc...>
    {};

    template<typename... Contents_t>
    using Serialize = typename serialize_meta<Contents_t...>::type;

    template<typename Type_t, Type_t... a1, Type_t... a2>
    Serialize<Content<Type_t, a1...>, Content<Type_t, a2...> > operator+(const Content<Type_t, a1...> &a, const Content<Type_t, a2...> &b);

    template<typename T_t, size_t s, const T_t (&lstr)[s]>
    using UnSerialize = typename unserialize_meta<T_t, s, lstr>::type;

    template<typename TagName>
    using GetName = typename TagName::name_t;
}
#define MAKE_STRING(label, str) \
    static constexpr char label[] = str; \
    using label##_t = meta::UnSerialize<char, sizeof(label), label>

#define MAKE_WSTRING(label, str) \
    static constexpr wchar_t label[] = str; \
    using label##_t = meta::UnSerialize<wchar_t, sizeof(label)/sizeof(wchar_t), label>
