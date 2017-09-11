#pragma once

constexpr bool b_or()
{
    return false;
}

template<typename... Bool>
constexpr bool b_or(bool a, Bool... etc)
{
    return a || b_or(etc...);
}


constexpr bool b_and()
{return true;}

template<typename... Bool>
constexpr bool b_and(bool val, Bool... arg)
{return val && b_and(arg...);}


template<typename Type_t, Type_t min, Type_t max>
constexpr bool isInside(Type_t value)
{
    return min <= value && value <= max;
}

template<typename Type_t, bool(&...callable)(Type_t)>
constexpr bool isInsideAny(Type_t value)
{
    return bor(callable(value)...);
}

namespace meta
{
    struct Nothing
    {};
}
