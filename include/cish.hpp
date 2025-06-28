#pragma once

#include <cish/symtab.hpp>

#include <stddef.h>
#include <stdint.h>


namespace cish
{
    struct Token;
    struct Node;

    struct CompileCtx
    {
        void *strtab;
        void *symtab;

    };

    
    size_t lexerMain( const char *src, Token *buf, size_t bufsz );
    Node *parserMain( const char *src, Token *buf, size_t bufsz );
}