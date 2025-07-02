#pragma once
#include <stdio.h>
#include <stdint.h>


namespace cish
{
    struct CompileCtx
    {
    public:
        uint8_t *m_dataBase;
        uint8_t *m_dataEend;
        size_t   m_dataTop;
        FILE    *data_out;
        FILE    *text_out;

        void *strtab;
        void *symtab;

        uintptr_t resb( size_t size, size_t align );
        void writeText( const char *fmt, ... );
    };
}
