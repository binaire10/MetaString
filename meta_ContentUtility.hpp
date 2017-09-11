#pragma once

#include <type_traits>
#include "meta_Serialize.hpp"
#include "Content.hpp"

namespace meta
{
    template<typename Content_t>
    struct get_content_info;

    template<template<typename T_t, T_t... str> class Content_t, typename Type_t, Type_t... val>
    struct get_content_info<Content_t<Type_t, val...> >
    {
        static constexpr unsigned long long DataSize = sizeof...(val);
        static constexpr unsigned long long Size = DataSize+1;
        typedef Type_t content_type;
        typedef Type_t (data_type)[Size];
    };

    template<typename Content_t>
    struct get_first_value_meta;

    template<template<typename T_t, T_t... str> class Content_t, typename Type_t, Type_t v, Type_t... val>
    struct get_first_value_meta<Content_t<Type_t, v, val...> >
    {
        constexpr static Type_t value = v;
    };

    template<typename Content_t>
    struct pop_front_content_meta
    {
        typedef Content_t type;
    };

    template<template<typename T_t, T_t... str> class Content_t, typename Type_t, Type_t d, Type_t... data>
    struct pop_front_content_meta<Content_t<Type_t, d, data...> >
    {
        typedef Content<Type_t, data...> type;
    };

    template<typename Content_t>
    using Pop_front_content = typename pop_front_content_meta<Content_t>::type;

    template<typename Content_t, typename get_content_info<Content_t>::content_type... value>
    using Add_end_value = Serialize<Content_t, Content<typename get_content_info<Content_t>::content_type, value...> >;

    template<typename Content_t, typename get_content_info<Content_t>::content_type... value>
    using Add_front_value = Serialize<Content<typename get_content_info<Content_t>::content_type, value...>, Content_t>;

    template<typename Content_t>
    constexpr typename get_content_info<Content_t>::content_type get_first_value = get_first_value_meta<Content_t>::value;

    template<typename Content_t, unsigned n, typename res_t = Content<typename get_content_info<Content_t>::content_type>, unsigned len = n>
    struct keep_front_content_meta : keep_front_content_meta<Pop_front_content<Content_t>, n, Add_end_value<res_t, get_first_value<Content_t> >, len-1>
    {};

    template<typename Content_t, unsigned n, typename res_t>
    struct keep_front_content_meta<Content_t, n, res_t, 0>
    {
        typedef res_t type;
    };

    template<typename Content_t, unsigned n>
    using Keep_front_content = typename keep_front_content_meta<Content_t,  n>::type;

    template<typename Content_t, unsigned n>
    using Extract_end = Keep_front_content<Content_t, get_content_info<Content_t>::DataSize?(get_content_info<Content_t>::DataSize-n):get_content_info<Content_t>::DataSize>;

    template<typename Content, unsigned beg>
    struct extract_begin_meta : extract_begin_meta<Pop_front_content<Content>, beg-1>
    {};

    template<typename Content>
    struct extract_begin_meta<Content, 0>
    {
        typedef Content type;
    };

    template<typename Content_t, unsigned n>
    using Extract_begin = typename extract_begin_meta<Content_t, n>::type;

    template<typename Content_t, unsigned beg, unsigned end = 0>
    using Extract = Extract_begin<Extract_end<Content_t, end>, beg>;

    template<typename Content_t, unsigned i>
    constexpr typename get_content_info<Content_t>::content_type get_i_value = get_first_value<Extract_begin<Content_t, i> >;

    template<typename Content_t>
    constexpr typename get_content_info<Content_t>::content_type get_last_value = get_first_value<Extract_begin<Content_t, get_content_info<Content_t>::DataSize-1> >;

    static_assert((get_first_value<Pop_front_content<Add_end_value<Content<char, 'a'>, 'b'> > > == 'b'), "binaire api test fail");
    static_assert((std::is_same<Content<char, 'a', 'k'>, Extract<Content<char, '7', '1', 'a', 'k', '5', '6'>, 2, 2> >::value), "binaire api test fail");
    static_assert((std::is_same<Content<char, 'a', 'k'>, Extract<Content<char, '7', '1', 'a', 'k', '5', '6', 'd'>, 2, 3> >::value), "binaire api test fail");
    static_assert((std::is_same<Content<char, 'a', 'k'>, Extract<Content<char, 'r', '7', '1', 'a', 'k', '5', '6'>, 3, 2> >::value), "binaire api test fail");
    static_assert((std::is_same<Content<char, 'a', 'k'>, Extract_end<Content<char, 'a', 'k', '6', '5', '6'>, 3>>::value), "binaire api test fail");
    static_assert((std::is_same<Content<char, 'a', 'k'>, Keep_front_content<Content<char, 'a', 'k', '6', '5', '6'>, 2> >::value), "binaire api test fail");
    static_assert((std::is_same<Content<char, 'r', '7', '1', 'a', 'k'>, Extract_end<Content<char, 'r', '7', '1', 'a', 'k', '5', '6'>, 2> >::value), "binaire api test fail");
    static_assert((get_i_value<Content<char, 'a', 'k', '6', '5', '6'>, 0> == 'a'), "binaire api test fail");
    static_assert((get_i_value<Content<char, 'a', 'k', '6', '5', '6'>, 3> == '5'), "binaire api test fail");
    static_assert(((get_last_value<Content<char, 'a', 'k', '6', '5', '6'> >) == '6'), "binaire api test fail");

    using space = Content<char, ' '>;
    namespace wide
    {
        using space = Content<wchar_t, L' '>;
    }
    template<typename... t>
    struct arg_size
    {
        enum {
            count = sizeof...(t)
        };
    };

    /*template<auto... t>
    struct argValue_size
    {
        enum {
            count = sizeof...(t)
        };
    };*/

}
