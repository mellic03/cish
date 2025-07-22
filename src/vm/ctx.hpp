#pragma once
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cish/stack.hpp>
#include <util/bitmanip.hpp>
#include <bit>


namespace cish
{
    struct VmOp;

    enum Reg_: uint8_t
    {
        Reg_rnul,
        Reg_rax, Reg_rbx, Reg_rcx, Reg_rdx,
        Reg_rsi, Reg_rdi, Reg_rsp, Reg_rbp,
        Reg_rip,

        Reg_rcmp0,  Reg_rcmp1,
        Reg_tmp,

        Reg_NumRegs,
    };


    struct VmCtx
    {
        VmOp    *text;
        uint8_t *m_stkmin;
        uint8_t *m_stkmax;

        union {
            int64_t regs[Reg_NumRegs];

            #define REG(lbl) union { int32_t e##lbl; int64_t r##lbl; }
            struct {
                REG(nul);
                REG(ax);  REG(bx); REG(cx); REG(dx);
                REG(si);  REG(di);
                uint8_t *rbp, *rsp;
                REG(ip);

                int64_t rcmp0,  rcmp1;
                int64_t rtmp;
            };
            #undef REG
        };

        VmCtx( VmOp *base, size_t size );

        // void callpush();
        // void callpop();

        template <typename T=uint64_t>
        void push( T x )
        {
            rsp -= sizeof(T);
            *(T*)(rsp) = x;
        }

        template <typename T=uint64_t>
        T pop()
        {
            rsp += sizeof(T);
            return *(T*)(rsp - sizeof(T));
        }

        template <typename T=uint64_t>
        void swap()
        {
            T A = pop<T>();
            T B = pop<T>();
            push<T>(A);
            push<T>(B);
        }

        template <typename T, typename U>
        void mov( T &dst, U src )
        {
            dst = std::bit_cast<T, U>(src);
        }

        template <typename T>
        T &global( int64_t offset )
        {
            return *(T*)(rbp - offset);
        }

        template <typename T>
        T &local( int64_t offset )
        {
            return *(T*)(rbp - offset);
        }

        // constexpr auto pushq = push<64, T>;
        // template <typename T>   void pushq( T x ) { push<64>(x); }
        // template <typename T>   T    popq() { return pop<64>(); }
        // template <typename T>   void swapq() { swap<64>(); }

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

