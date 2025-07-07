#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cish/type.hpp>


namespace cish
{
    struct Token
    {
        uint32_t type;
        char     lexeme[32];
        uint32_t lineno;
        uint32_t colno;

        Token() { };
        Token( uint32_t tp );
        Token( uint32_t tp, const char *str );
        Token( uint32_t tp, const char *str, uint32_t line, uint32_t col );
    };
}
