#pragma once
#include <stddef.h>

template <typename T>
constexpr T align_up( T value, size_t align )
{
    return T(((size_t)value + (align-1)) & ~(align-1));
}


template <typename T>
inline constexpr T align_down( T x, T A )
{
    return T( int64_t(x) & -int64_t(A) );
}
