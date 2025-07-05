#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    enum VmOp_: uint64_t
    {
        VmOp_None,

        VmOp_push,  VmOp_pop,
        VmOp_pushn, VmOp_popn,

        VmOp_addu, VmOp_subu, VmOp_mulu, VmOp_divu,
        VmOp_addi, VmOp_subi, VmOp_muli, VmOp_divi,

        VmOp_and,  VmOp_or,   VmOp_xor,
        VmOp_neg,  VmOp_not,

        VmOp_jmp,  VmOp_rjmp,
        VmOp_call, VmOp_ret,
        VmOp_exit,

        VmOp_print,

        VmOp_NumOps,
    };

    // enum VmOp_: uint8_t
    // {
    //     VmOp_Unused,

    //     VmOp_reg_val = 0b0010, // reg_lit, [opcode, dst, 0x00, 0x00], [value], [opcode], ...
    //     VmOp_reg_reg = 0b0011, // reg_reg, [opcode, dst,  src, 0x00], [opcode, ... ]

    //     VmOp_mov = 0b0100,

    //     VmOp_add,
    //     VmOp_sub,
    //     VmOp_mul,
    //     VmOp_div,

    //     VmOp_and,
    //     VmOp_or,
    //     VmOp_xor,

    //     VmOp_push,
    //     VmOp_pop,

    //     VmOp_jmp,
    //     VmOp_call,
    //     VmOp_ret,
    //     VmOp_exit,

    //     VmOp_NumOps,
    // };

    // static constexpr uint8_t VmOpMode_LIT  = 0b00;
    // static constexpr uint8_t VmOpMode_REG  = 0b01;



    union VmOp
    {
        uint64_t qword;

        struct
        {
            uint8_t opcode;
            uint8_t resv0;
            uint8_t resv1;
            uint8_t resv2;
            uint32_t data;
        };

        // struct
        // {
        //     uint8_t opcode :7;
        //     uint8_t sign   :1;
        //     // uint8_t big    :1;
        //     // uint8_t dstIsReg :1;
        //     // uint8_t srcIsReg :1;
        //     uint8_t B1;
        //     uint8_t B2;
        //     uint8_t B3;
        //     uint32_t data;

        // } __attribute__((packed));

    } __attribute__((packed)) __attribute__((aligned(8)));

}


