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
        Reg_r8,  Reg_r9,  Reg_r10, Reg_r11,
        Reg_rsi, Reg_rdi,
        Reg_rsp, Reg_rbp,
        Reg_rip,

        Reg_memptr,
        Reg_vsp,    Reg_vbp,
        Reg_rspoff, Reg_rbpoff,
        Reg_rcmp0,  Reg_rcmp1,
        Reg_tmp,

        Reg_NumRegs,
    };


    struct VmCtx
    {
        template <typename T, size_t Cap>
        using Stack = cish::fixedsize_stack<T, Cap>;

        VmOp     *text;
        uint32_t *stack;
        uint32_t *vstack;

        struct call_t { uint64_t ip, sp, bp, v_sp, v_bp; };
        Stack<call_t, 64> callstack;

        union {
            uint64_t regs[Reg_NumRegs];

            #define REG(lbl) union { uint32_t e##lbl; uint64_t r##lbl; }
            struct {
                REG(nul);
                REG(ax);  REG(bx); REG(cx); REG(dx);
                REG(8);   REG(9);  REG(10); REG(11);
                REG(si);  REG(di);
                REG(sp);  REG(bp);
                REG(ip);

                uint64_t memptr;
                uint64_t vsp, vbp;
                uint64_t rspoff, rbpoff;
                uint64_t rcmp0,  rcmp1;
                uint64_t rtmp;
            };
            #undef REG
        };

        VmCtx( uint32_t *program, size_t size );

        void jmp( uint32_t addr );
        void rjmp( uint32_t addr );

        void callpush();
        void callpop();

        // // template <typename T>
        // void vpush( uint32_t value )
        // {
        //     vstack[vsp++] = value;
        //     // *(T*)(vstack + vsp) = value;
        //     // vsp += sizeof(T);
        // }

        // // template <typename T>
        // uint32_t vpop()
        // {
        //     vsp--;
        //     return vstack[vsp];
        //     // vsp -= sizeof(T);
        //     // return *(T*)(vstack + vsp);
        // }

        void vpush( uint32_t value )
        {
            vstack[vsp++] = value;
        }

        uint32_t vpop()
        {
            return vstack[--vsp];
        }

        void push( uint32_t value )
        {
            stack[rsp++] = value;
        }

        uint32_t pop()
        {
            return stack[--rsp];
        }

        uint32_t &top()
        {
            return stack[rsp-1];
        }

        // template <typename T>
        // T &top()
        // {
        //     return *(T*)(stack+rsp - sizeof(T));
        // }

        void swap()
        {
            uint32_t A = pop();
            uint32_t B = pop();
            push(A);
            push(B);
        }

        // template <typename T>
        // T &local(size_t offset )
        // {
        //     // union {
        //     //     uint8_t *as_u8;
        //     //     T       *as_T;
        //     // } U = {stack + rbp + rbpoff};
        //     return *(T*)(stack + rbp + offset);
        // }
    };
}

