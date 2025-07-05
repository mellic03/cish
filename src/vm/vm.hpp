#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    union VmOp;

    struct VmCtx
    {
        VmOp     *txt;
        uint64_t  rip;
        uint64_t  rbp;
        uint64_t *stack;
        uint64_t  rsp;

        VmCtx( uint64_t *program, size_t size );

        void push( uint64_t x )
        {
            stack[rsp++] = x;
        }

        uint64_t pop()
        {
            return stack[--rsp];
        }

        uint64_t top()
        {
            return stack[rsp-1];
        }

    };

    int exec( uint64_t*, size_t );
}

