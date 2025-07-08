#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    enum VmOpMd_: uint8_t
    {
        // VmOpMd_i = 0b01,
        // VmOpMd_r = 0b10,
        // VmOpMd_l = 0b11,

        // i_ = 0b01'00,
        // _i = 0b00'01,

        // r_ = 0b10'00,
        // _r = 0b00'10,

        // l_ = 0b11'00,
        // _l = 0b00'11,

        //   d s i --> dst src imm
        // 0 0 0 0

        VmOpMd_xxi,

        VmOpMd_xr,
        VmOpMd_rx,
        VmOpMd_ri,
        VmOpMd_rr,
        VmOpMd_rl,

        VmOpMd_xl,
        VmOpMd_lx,
        VmOpMd_li,
        VmOpMd_lr,
        // VmOpMd_ll,
    };

    enum VmOp_: uint8_t
    {
        VmOp_nop = 0,

        // VmOp_mov,
        // VmOp_mov_rr,
        // VmOp_mov_ri,
        // VmOp_mov_rl,
        // VmOp_mov_lr,

        // VmOp_add_rr, VmOp_add_ri,
        // VmOp_sub_rr, VmOp_sub_ri,
        // VmOp_mul_rr, VmOp_mul_ri,
        // VmOp_div_rr, VmOp_div_ri,

        // VmOp_push_r, VmOp_push_i,
        // VmOp_pop_r,

        VmOp_mov_rxi,
        VmOp_mov_rrx,
        VmOp_add_rxi,
        VmOp_add_rrx,

        VmOp_gload_xi,
        VmOp_gstor_ix,
        VmOp_vload_xi,
        VmOp_vstor_ix,

        VmOp_push_xxi,
        VmOp_push_xrx,
        VmOp_pop_rxx,
        VmOp_swap,

        VmOp_add,
        VmOp_sub,
        VmOp_mul,
        VmOp_div,
        VmOp_and,
        VmOp_or,
        VmOp_xor,
        VmOp_not,
        VmOp_neg,

        VmOp_equ,
        VmOp_les,
        VmOp_leq,
        VmOp_gtr,
        VmOp_geq,

        VmOp_jmp,  // jmp imm
        VmOp_jeq,  // if rcmp0==rcmp1: jmp imm
        VmOp_jne,  // if rcmp0!=rcmp1: jmp imm

        VmOp_call,
        VmOp_ret,
        VmOp_exit,

        VmOp_CompileIR,

        VmOp_NumOps,
    };


    struct VmOp
    {
        struct {
            uint8_t opcode;
            uint8_t dstreg;
            uint8_t srcreg;
            uint8_t B3;
        };
    
        union {
            uint8_t  d08;
            uint16_t d16;
            uint32_t d32;
            uint32_t imm;
        };

        VmOp( VmOp_ type, uint8_t dst, uint8_t src, uint32_t value )
        :   opcode(type), dstreg(dst), srcreg(src), imm(value) {  }

        VmOp( VmOp_ type, uint32_t value=0 )
        :   VmOp(type, 0, 0, value) {  }

        VmOp( VmOp_ type, uint8_t A, uint8_t B )
        :   VmOp(type, A, B, 0) {  }

    } __attribute__((aligned(4)));

    // inline constexpr
    // VmOp VmOpReg( VmOp_ type, uint8_t A )
    // {
    //     return VmOp(type, A, 0, 0);
    // }

    // inline constexpr
    // VmOp VmOpRegReg( VmOp_ type, uint8_t A, uint8_t B )
    // {
    //     return VmOp(type, A, B, 0);
    // }

    // inline constexpr
    // VmOp VmOpImm( VmOp_ type, uint32_t value )
    // {
    //     return VmOp(type, 0, 0, value);
    // }

    // inline constexpr
    // VmOp VmOpRegImm( VmOp_ type, uint8_t A, uint32_t value )
    // {
    //     return VmOp(type, A, 0, value);
    // }

    // inline constexpr
    // VmOp VmOpRegRegImm( VmOp_ type, uint8_t A, uint8_t B, uint32_t value )
    // {
    //     return VmOp(type, A, B, value);
    // }

}


