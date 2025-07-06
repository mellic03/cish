#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    struct VmOp;

    enum Reg_: uint8_t
    {
        Reg_rax, Reg_rbx, Reg_rcx, Reg_rdx,
        Reg_rip, Reg_rsp, Reg_rbp,
    };

    struct VmCtx
    {
        VmOp     *txt;
        uint64_t *stack;

        union {
            uint64_t regs[8];
            struct {
                uint64_t rax, rbx, rcx, rdx;
                uint64_t rip, rsp, rbp;
            };
        };

        VmCtx( uint32_t *program, size_t size );

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

    int exec( uint32_t*, size_t );
}

