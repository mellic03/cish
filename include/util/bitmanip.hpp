#pragma once
#include <stddef.h>

template <typename T>
constexpr T align_up( T value, size_t align )
{
    return T(((size_t)value + (align-1)) & ~(align-1));
}