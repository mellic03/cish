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

        OpRI_mov,
        OpRR_mov,
        OpRI_add,
        OpRR_add,

        OpXI_gload,
        OpIX_gstor,
        OpXI_vload,
        OpIX_vstor,

        OpI_push,
        OpR_push,
        OpR_pop,
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





    // enum OpMd_: uint8_t
    // {
    //     OpMd_X, OpMd_R, OpMd_I, OpMd_P
    // };
    
    // struct VmInst
    // {
    //     uint8_t opcode;
    //     // VmOpXX_add
    //     // VmOpRR_add
    //     // VmOpRI_add
    //     // VmOpRP_add
    //                       // none, reg, imm, ptr
    //     uint8_t xmode :2; // [0-3] --> x, r, i, p
    //     uint8_t ymode :2;
    //     uint8_t xsize :1; // 0/1 --> 32/64
    //     uint8_t ysize :1;
    //     uint8_t sign  :1;
    //     uint8_t resv  :1;

    //     uint8_t regx :4;
    //     uint8_t regy :4;

    //     uint8_t regz :4;
    //     uint8_t regw :4;
    // };
    
}


