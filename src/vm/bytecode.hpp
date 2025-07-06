#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    enum VmOp_: uint64_t
    {
        VmOp_nop,

        // VmOp_xxrd:  mov <A>, data
        // VmOp_xxrr:  mov <A>, <B>
        // VmOp_xxrrd: mov <A>, stack[<B> + data]
        // VmOp_xxrdr: mov stack[<A> + data], <B>

        VmOp_movrd, VmOp_movrr, VmOp_movrrd, VmOp_movrdr,
        VmOp_pushd, VmOp_pop,   VmOp_pushr,  VmOp_popr,
        VmOp_pushrd,
    
        VmOp_addrd, VmOp_subrd, VmOp_mulrd,  VmOp_divrd,
        VmOp_andrd, VmOp_orrd,  VmOp_xorrd,

        VmOp_addrr, VmOp_subrr, VmOp_mulrr,  VmOp_divrr,
        VmOp_andrr, VmOp_orrr,  VmOp_xorrr,

        VmOp_adds,  VmOp_subs,  VmOp_muls,   VmOp_divs,
        VmOp_ands,  VmOp_ors,   VmOp_xors,

        VmOp_jmpd,  VmOp_jmpr,  VmOp_rjmpd,  VmOp_rjmpr,
        VmOp_calld, VmOp_callr, VmOp_ret,

        // VmOp_addu, VmOp_subu, VmOp_mulu,  VmOp_divu,
        // VmOp_addi, VmOp_subi, VmOp_muli,  VmOp_divi,
        // VmOp_and,  VmOp_or,   VmOp_xor,
        // VmOp_neg,  VmOp_not,
        // VmOp_jmp,  VmOp_rjmp,
        // VmOp_call, VmOp_ret,

        VmOp_exit,

        // VmOp_print,

        VmOp_NumOps,
    };

    union VmOpU64LoHi
    {
        uint64_t u64;
        struct {
            uint32_t lo;
            uint32_t hi;
        };
    };

    struct VmOpU64
    {
        uint32_t lo;
        uint32_t hi;

        VmOpU64( uint64_t value )
        {
            VmOpU64LoHi U = {value};
            this->lo = U.lo;
            this->hi = U.hi;
        }
    };

    struct VmOpData
    {
        VmOpData( uint64_t value )
        :   data(value) {  }

        uint8_t opcode;
        uint8_t B1;
        uint8_t B2;
        uint8_t B3;
        VmOpU64 data;
        uint32_t next[];
    };

    struct VmOpReg
    {
        VmOpReg( uint8_t regIdx )
        :   idx(regIdx) {  }

        uint8_t opcode;
        uint8_t idx;
        uint8_t B2;
        uint8_t B3;
        uint32_t next[];
    };


    struct VmOpRegData
    {
        VmOpRegData( uint8_t regIdx, uint64_t value )
        :   idx(regIdx), data(value) {  }

        uint8_t opcode;
        uint8_t idx;
        uint8_t B2;
        uint8_t B3;
        VmOpU64 data;
        uint32_t next[];
    };


    struct VmOpRegReg
    {
        VmOpRegReg( uint8_t regA, uint8_t regB )
        :   idxA(regA), idxB(regB) {  }

        uint8_t opcode;
        uint8_t idxA;
        uint8_t idxB;
        uint8_t B3;
        uint32_t next[];
    };


    struct VmOpRegRegData
    {
        VmOpRegRegData( uint8_t regA, uint8_t regB, uint64_t value )
        :   idxA(regA), idxB(regB), data(value) {  }

        uint8_t opcode;
        uint8_t idxA;
        uint8_t idxB;
        uint8_t B3;
        VmOpU64 data;
        uint32_t next[];
    };

    struct VmOpRegDataReg
    {
        VmOpRegDataReg( uint8_t regA, uint64_t value, uint8_t regB )
        :   idxA(regA), idxB(regB), data(value) {  }

        uint8_t opcode;
        uint8_t idxA;
        uint8_t idxB;
        uint8_t B3;
        VmOpU64 data;
        uint32_t next[];
    };


    struct VmOp
    {
        uint8_t opcode;
        uint8_t B1;
        uint8_t B2;
        uint8_t B3; // 4 bytes
        uint32_t next[];
    };

    // template <typename T>
    // VmOp newVmOp( uint8_t opcode, const T &data )
    // {
    //     VmOp op = {opcode};
    //     new (op) T(data);
    //     return op;
    // }

    // union VmOp
    // {
    //     uint64_t qword;

    //     struct
    //     {
    //         uint8_t opcode;
    //         uint8_t resv0;
    //         uint8_t resv1;
    //         uint8_t resv2;
    //         uint32_t data;
    //     };

    //     // struct
    //     // {
    //     //     uint8_t opcode :7;
    //     //     uint8_t sign   :1;
    //     //     // uint8_t big    :1;
    //     //     // uint8_t dstIsReg :1;
    //     //     // uint8_t srcIsReg :1;
    //     //     uint8_t B1;
    //     //     uint8_t B2;
    //     //     uint8_t B3;
    //     //     uint32_t data;

    //     // } __attribute__((packed));

    // } __attribute__((packed)) __attribute__((aligned(8)));

}


