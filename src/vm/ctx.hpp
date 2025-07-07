#pragma once
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cish/stack.hpp>
#include <util/bitmanip.hpp>

namespace cish
{
    struct VmOp;

    enum Reg_: uint8_t
    {
        Reg_rnul,
        Reg_rax, Reg_rbx, Reg_rcx, Reg_rdx,
        Reg_rsi, Reg_rdi,
        Reg_rsp, Reg_rbp,
        Reg_rip,
        Reg_vsp, Reg_vbp,
    };


    struct VmCtx
    {
        template <typename T, size_t Cap>
        using Stack = cish::fixedsize_stack<T, Cap>;

        VmOp    *text;
        uint8_t *stack;
        uint32_t vstack[1024]; // __attribute__((aligned(4)));

        struct call_t { uint64_t ip, sp, bp; };
        Stack<call_t, 64> callstack;

        union {
            uint64_t regs[12];

            #define REG(lbl) union { uint32_t e##lbl; uint64_t r##lbl; }
            struct {
                REG(nul);
                REG(ax);  REG(bx); REG(cx); REG(dx);
                REG(si);  REG(di);
                REG(sp);  REG(bp);
                REG(ip);
                uint64_t vsp, vbp;

            };
            #undef REG
        };

        VmCtx( uint32_t *program, size_t size );

        void jmp( uint32_t addr );
        void rjmp( uint32_t addr );

        // template <typename T>
        void vpush( uint32_t value )
        {
            vstack[vsp++] = value;
            // *(T*)(vstack + vsp) = value;
            // vsp += sizeof(T);
        }

        // template <typename T>
        uint32_t vpop()
        {
            vsp--;
            return vstack[vsp];
            // vsp -= sizeof(T);
            // return *(T*)(vstack + vsp);
        }

        template <typename T>
        void push( T value )
        {
            *(T*)(stack+rsp) = value;
            rsp += sizeof(T);
        }

        template <typename T>
        T pop()
        {
            rsp -= sizeof(T);
            return *(T*)(stack+rsp);
        }

        template <typename T>
        T &top()
        {
            return *(T*)(stack+rsp - sizeof(T));
        }

        template <typename T>
        void swap()
        {
            T A = pop<T>();
            T B = pop<T>();
            push(A);
            push(B);
        }
    };
}

