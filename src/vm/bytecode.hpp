#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    enum class MD: uint8_t
    {
        nul = 0b00,
        imm = 0b01,
        reg = 0b10,
        ptr = 0b11,
    };

    enum VmOp_: uint8_t
    {
        VmOp_nop = 0,

        OpI_prnt,
        OpR_prnt,

        OpRI_mov,
        OpRI_add, OpRI_sub, OpRI_mul, OpRI_div,

        OpRR_mov,
        OpRR_add, OpRR_sub, OpRR_mul, OpRR_div,


        //          0000 0000 0000 0001
        // << 11    0001 0000 0000 0000

        //          0000 0000 0000 0011
        // << 10    0000 1100 0000 0000

        //          0000 0000 0000 0011
        // << 8     0000 0011 0000 0000

        //          0001 1111 0000 0000
        //
        // 0000 0110 0000 0000
        // 0001 1000 0000 0000
        // 000x xyys 0000 0000
        // 
        // op_rru_add = (op_signed) | (MD::reg<<10) | (MD::reg<<8) | op_xys_add,

        OpI_gload,
        OpI_gstor,
        OpI_lload,
        OpI_lstor,

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


        OP_xx_mov,
        OP_xx_gmov,
        // OP_mr_lmov, // mov rax, [rbp - imm] 

        OP_xxx_add,
        OP_xxx_sub,
        OP_xxx_mul,
        OP_xxx_div,

        OP_X_gload,
        OP_X_gstor,
    };

    #define VM_INSTRUCTION_BODY \
        struct { \
            uint8_t is_signed :1; \
            uint8_t xmode     :2; /* [0-3] --> X, R, I, P*/ \
            uint8_t ymode     :2; \
            uint8_t resv      :3; \
            uint8_t opcode    :8; \
        } __attribute__((packed))

    union VmInstruction
    {
        uint16_t as_u16;
        VM_INSTRUCTION_BODY;

        VmInstruction() {  }
        VmInstruction( uint8_t op, uint8_t sign, MD xm, MD ym )
        : is_signed(sign), opcode(op), xmode((uint8_t)xm), ymode((uint8_t)ym) {  }
    } __attribute__((packed));

    struct VmOp
    {
        union {
            VmInstruction inst;
            VM_INSTRUCTION_BODY;
        } __attribute__((packed));

        uint8_t dstreg;
        uint8_t srcreg;

        union {
            uint32_t immu;
            int32_t  imm;
        };

        VmOp( VmOp_ type, uint8_t dst, uint8_t src, int32_t value )
        :   opcode(type), dstreg(dst), srcreg(src), imm(value) {  }

        VmOp( VmOp_ type, int32_t value=0 )
        :   VmOp(type, 0, 0, value) {  }

        VmOp( VmOp_ type, uint8_t A, uint8_t B )
        :   VmOp(type, A, B, 0) {  }

        VmOp()
        : VmOp(VmOp_nop, 0, 0, 0) {  };

    } __attribute__((packed));


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


