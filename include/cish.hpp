#pragma once

#include <cish/symtab.hpp>

#include <stddef.h>
#include <stdint.h>


namespace cish
{
    struct Token;
    struct iNode;

    size_t lexerMain( const char *src, Token *buf, size_t bufsz );
    iNode *parserMain( const char *src, Token *buf, size_t bufsz );
}