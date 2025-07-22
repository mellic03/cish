#pragma once

#include <cish/symbol.hpp>

#include <stddef.h>
#include <stdint.h>


namespace cish
{
    struct Token;
    struct Node;

    size_t lexerMain( const char *src, Token *buf, size_t bufsz );
    Node *parserMain( const char *src, Token *buf, size_t bufsz );
}