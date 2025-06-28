#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cish/type.hpp>


namespace cish
{
    struct Token
    {
        uint32_t type;
        uint32_t line;
        char     lexeme[32];

        Token() { };
        Token( uint32_t tp );
        Token( uint32_t tp, const char *str );
    };
}
