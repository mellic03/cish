#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


namespace cish
{
    enum VmOp_: uint8_t
    {
        VmOp_nop,

        // VmOp_stor32rr, // mov [rsp + <A>], <B>
        // VmOp_stor32rd, // mov [rsp + <A>], d32
        // VmOp_load32rr, // mov <A>, [rsp + <B>]
        // VmOp_load32rd, // mov <A>, [rsp + d32]
        // VmOp_stor, // mov [rsp + <A>], <B>
        // VmOp_load, // mov <A>, [rsp + <B>]


        // VmOp_vstor32rr, // mov [varstack + <A>], <B>
        // VmOp_vstor32rd, // mov [varstack + <A>], d32
        // VmOp_vload32rr, // mov <A>, [varstack + <B>]
        // VmOp_vload32rd, // mov <A>, [varstack + d32]
        VmOp_vstor,
        VmOp_vload,
        VmOp_vpush32d,
        VmOp_vpop32r,
        // VmOp_vpushrd, VmOp_vpush08, VmOp_vpush16, VmOp_vpush32,
        // VmOp_vpoprg,  VmOp_vpop08,  VmOp_vpop16,  VmOp_vpop32, 

        VmOp_movrg, VmOp_mov08, VmOp_mov16, VmOp_mov32,
        VmOp_addrg, VmOp_add08, VmOp_add16, VmOp_add32,
        VmOp_subrg, VmOp_sub08, VmOp_sub16, VmOp_sub32,
        VmOp_mulrg, VmOp_mul08, VmOp_mul16, VmOp_mul32,
        VmOp_divrg, VmOp_div08, VmOp_div16, VmOp_div32,

        VmOp_push08r, VmOp_push16r, VmOp_push32r,
        VmOp_push08d, VmOp_push16d, VmOp_push32d,
        VmOp_pop08r,  VmOp_pop16r,  VmOp_pop32r,
        VmOp_swap08,  VmOp_swap16, VmOp_swap32,

        VmOp_add,  // push( pop() + pop() )
        VmOp_sub,
        VmOp_mul,
        VmOp_div,
        VmOp_and,
        VmOp_or,
        VmOp_xor,
        VmOp_not,  // push( ~pop() )
        VmOp_neg,  // push( 1 - pop() )

        VmOp_equ,  // push( pop() == pop() )
        VmOp_les,  // push( pop() <  pop() )
        VmOp_leq,  // push( pop() <= pop() )
        VmOp_gtr,  // push( pop() >  pop() )
        VmOp_geq,  // push( pop() >= pop() )

        VmOp_jmp,  // jmp dataw --> mov rip, dataw
        VmOp_jeq,  // jmp dataw if [rsp-1] == [rsp-2]
        VmOp_jne,  // jmp dataw if [rsp-1] != [rsp-2]

        VmOp_rjmp, // rjmp dataw --> add rip, dataw
        VmOp_rjeq,
        VmOp_rjne,

        VmOp_call,
        VmOp_ret,
        VmOp_exit,

        VmOp_NumOps,
    };


    struct VmOp
    {
        struct {
            uint8_t opcode;
            uint8_t regA;
            uint8_t regB;
            uint8_t B3;
        };
    
        union {
            uint8_t  d08;
            uint16_t d16;
            uint32_t d32;
            uint32_t data;
        };

        VmOp( VmOp_ type, uint8_t A, uint8_t B, uint32_t value )
        :   opcode(type), regA(A), regB(B), data(value) {  }

        VmOp( VmOp_ type, uint32_t value=0 )
        :   VmOp(type, 0, 0, value) {  }

        VmOp( VmOp_ type, uint8_t A, uint8_t B )
        :   VmOp(type, A, B, 0) {  }

    } __attribute__((aligned(4)));


    inline constexpr
    VmOp VmOpReg( VmOp_ type, uint8_t A )
    {
        return VmOp(type, A, 0, 0);
    }

    inline constexpr
    VmOp VmOpRegReg( VmOp_ type, uint8_t A, uint8_t B )
    {
        return VmOp(type, A, B, 0);
    }

    inline constexpr
    VmOp VmOpData( VmOp_ type, uint32_t value )
    {
        return VmOp(type, 0, 0, value);
    }

    inline constexpr
    VmOp VmOpRegData( VmOp_ type, uint8_t A, uint32_t value )
    {
        return VmOp(type, A, 0, value);
    }

    inline constexpr
    VmOp VmOpRegRegData( VmOp_ type, uint8_t A, uint8_t B, uint32_t value )
    {
        return VmOp(type, A, B, value);
    }

}


