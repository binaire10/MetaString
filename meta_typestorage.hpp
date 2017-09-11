#pragma once
#include <type_traits>
#include <meta_Utility.hpp>

template<typename...>
struct Union_meta;
template<typename P>
struct Union_meta<P>
{
    using type = P;
};

template<template<typename...> class X, typename... a1, typename... a2, typename... z>
struct Union_meta<X<a1...>, X<a2...>, z...> : Union_meta<X<a1..., a2...>, z...>
{};

template<typename... a>
using Union = typename Union_meta<a...>::type;

template<typename Type_t>
struct TypeStore
{
    using type = Type_t;
};

template<template<typename...> class Type_t>
struct TemplateTypeStore
{
    template<typename... arg>
    using type = Type_t<arg...>;
};

template<typename Type, typename Storage>
struct ContainType_meta;

template<typename Type, template<typename...> class Storage, typename... arg>
struct ContainType_meta<Type, Storage<arg...> >
{
    static constexpr bool value = b_or(std::is_same<Type, arg>::value...);
};

template<typename Type, typename Storage>
constexpr bool containType = ContainType_meta<Type, Storage>::value;

static_assert((containType<char, std::tuple<int, char, int , char> >)==true, "size fail");
static_assert((containType<double, std::tuple<int, char, int , char> >)==false, "size fail");

template<typename Storage>
struct PopFront_meta
{
    using type = Storage;
};

template<typename Type, template<typename...> class Storage, typename... arg>
struct PopFront_meta<Storage<Type, arg...> >
{
    using type = Storage<arg...>;
};

template<typename Storage>
using PopFront = typename PopFront_meta<Storage>::type;

template<typename Type, typename Storage>
struct AddFront_meta;

template<typename Type, template<typename...> class Storage, typename... arg>
struct AddFront_meta<Type, Storage<arg...> >
{
    using type = Storage<Type, arg...>;
};

template<typename Type, typename Storage>
using AddFront = typename AddFront_meta<Type, Storage>::type;

template<typename Type, typename Storage>
struct AddBack_meta;

template<typename Type, template<typename...> class Storage, typename... arg>
struct AddBack_meta<Type, Storage<arg...> >
{
    using type = Storage<arg..., Type>;
};

template<typename Type, typename Storage>
using AddBack = typename AddBack_meta<Type, Storage>::type;

template<typename Storage>
struct StorageType_info
{
    static constexpr unsigned long long size = 0;
    using type = Storage;
};

template<template<typename...> class Storage, typename... a1>
struct StorageType_info<Storage<a1...> >
{
    static constexpr unsigned long long size = sizeof...(a1);
    template<typename... a>
    using type = Storage<a...>;
};

static_assert(StorageType_info<std::tuple<int, char, int , char> >::size==4, "size fail");
static_assert(std::is_same<std::tuple<>, StorageType_info<std::tuple<int, char, int , char> >::type<> >::value, "size fail");

template<typename Storage, typename Result = typename StorageType_info<Storage>::template type<> >
struct MakeSet_meta
{
    using type = Result;
};

template<template<typename...> class Storage, typename Type, typename... a1, typename Result>
struct MakeSet_meta<Storage<Type, a1...>, Result> : MakeSet_meta<Storage<a1...>, typename std::conditional<ContainType_meta<Type, Result>::value, Result, typename AddBack_meta<Type, Result>::type>::type >
{};

template<typename Storage>
using MakeSet = typename MakeSet_meta<Storage>::type;

template<typename... TypeStores_t>
struct Store
{};
