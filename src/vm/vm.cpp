#include "vm.hpp"
#include "ctx.hpp"
#include "bytecode.hpp"
#include <util/bitmanip.hpp>
#include <bit>
#include <stdio.h>
#include <string.h>

using namespace cish;


extern const char *VmOpStr( uint8_t opcode );
extern const char *VmRegStr( uint8_t );

int cish::exec( VmOp *base, size_t size )
{
    void *jtab[256] = {
        &&op_nop,

        &&op_i_prnt,
        &&op_r_prnt,

        &&op_ri_mov, &&op_ri_add, &&op_ri_sub, &&op_ri_mul, &&op_ri_div,
        &&op_rr_mov, &&op_rr_add, &&op_rr_sub, &&op_rr_mul, &&op_rr_div,

        &&op_i_gload,
        &&op_i_gstor,
        &&op_i_lload,
        &&op_i_lstor,

        &&op_i_push,
        &&op_r_push,
        &&op_r_pop,
        &&op_swap,

        &&op_add,
        &&op_sub,
        &&op_mul,
        &&op_div,
        &&op_and,
        &&op_or,
        &&op_xor,
        &&op_not,
        &&op_neg,

        &&op_equ, 
        &&op_les,
        &&op_leq,
        &&op_gtr,
        &&op_geq,
    
        &&op_jmp,
        &&op_jeq,
        &&op_jne,

        &&op_call,
        &&op_ret,
        &&op_exit,

        &&op_CompileIR,

    };

    for (int i=VmOp_NumOps; i<256; i++)
    {
        jtab[i] = &&op_error;
    }

    VmCtx ctx(base, size);
    auto *op = (VmOp*)(ctx.text);

    #define DST_REG           ctx.regs[op->dstreg]
    #define SRC_REG           ctx.regs[op->srcreg]
    #define BINARY_OP(op_)  { ctx.push(ctx.pop<int32_t>() op_ ctx.pop<int32_t>()); }
    #define UNARY_OP(op_)   { ctx.push( op_ ctx.pop<int32_t>() ); }
    goto DISPATCH;

DISPATCH:
    op=&ctx.text[ctx.rip++];
    printf("[%lu] %s %s %s %d\n", (uint64_t)(ctx.rip)-1, VmOpStr(op->opcode), VmRegStr(op->dstreg), VmRegStr(op->srcreg), op->imm);
    goto *jtab[op->opcode];

op_nop:    ctx.rnul *= 0; goto DISPATCH;
op_i_prnt: printf("%d\n", op->imm);                           goto DISPATCH;
op_r_prnt: printf("%s %ld\n", VmRegStr(op->dstreg), DST_REG); goto DISPATCH;

op_ri_mov: DST_REG  = op->imm; goto DISPATCH;
op_ri_add: DST_REG += op->imm; goto DISPATCH;
op_ri_sub: DST_REG -= op->imm; goto DISPATCH;
op_ri_mul: DST_REG *= op->imm; goto DISPATCH;
op_ri_div: DST_REG /= op->imm; goto DISPATCH;
op_rr_mov: DST_REG  = SRC_REG; goto DISPATCH;
op_rr_add: DST_REG += SRC_REG; goto DISPATCH;
op_rr_sub: DST_REG -= SRC_REG; goto DISPATCH;
op_rr_mul: DST_REG *= SRC_REG; goto DISPATCH;
op_rr_div: DST_REG /= SRC_REG; goto DISPATCH;

op_i_gload: ctx.push(ctx.local<int32_t>(op->imm));               goto DISPATCH;
op_i_gstor: ctx.local<int32_t>(op->imm) = ctx.pop<int32_t>();    goto DISPATCH;
op_i_lload: ctx.push(ctx.local<int32_t>(op->imm));               goto DISPATCH;
op_i_lstor: ctx.local<int32_t>(op->imm) = ctx.pop<int32_t>();    goto DISPATCH;

op_i_push: ctx.push(op->imm);            goto DISPATCH;
op_r_push: ctx.push(SRC_REG);            goto DISPATCH;
op_r_pop:  DST_REG = ctx.pop<int32_t>(); goto DISPATCH;
op_swap:   ctx.swap<int32_t>();          goto DISPATCH;

op_add:  BINARY_OP(+);  goto DISPATCH;
op_sub:  BINARY_OP(-);  goto DISPATCH;
op_mul:  BINARY_OP(*);  goto DISPATCH;
op_div:  BINARY_OP(/);  goto DISPATCH;
op_and:  BINARY_OP(&);  goto DISPATCH;
op_or:   BINARY_OP(|);  goto DISPATCH;
op_xor:  BINARY_OP(^);  goto DISPATCH;
op_not:  UNARY_OP (~);  goto DISPATCH;
op_neg:  UNARY_OP (-);  goto DISPATCH;
op_equ:  BINARY_OP(==); goto DISPATCH;
op_les:  BINARY_OP(<);  goto DISPATCH;
op_leq:  BINARY_OP(<);  goto DISPATCH;
op_gtr:  BINARY_OP(>);  goto DISPATCH;
op_geq:  BINARY_OP(>);  goto DISPATCH;

op_jmp: ctx.rip = op->imm;                          goto DISPATCH;
op_jeq: if (ctx.rcmp0==ctx.rcmp1) ctx.rip=op->imm;  goto DISPATCH;
op_jne: if (ctx.rcmp0!=ctx.rcmp1) ctx.rip=op->imm;  goto DISPATCH;

op_call: {
    printf("[op_call] %ld, %ld, %ld\n", ctx.rip, ctx.rbp, ctx.rsp);
    ctx.push(ctx.rbp);
    ctx.push(ctx.rip);
    ctx.rbp = ctx.rsp;
    ctx.rip = op->imm;
    // ctx.push(ctx.rip);
    // ctx.push(ctx.rbp);
    // ctx.rbp = ctx.rsp;
    // ctx.rip = op->imm;
}   goto DISPATCH;

op_ret: {
    if (ctx.rbp == ctx.m_stkmax)
        goto op_exit;
    ctx.rsp = ctx.rbp;
    ctx.rip = ctx.pop<int64_t>();
    ctx.rbp = ctx.pop<uint8_t*>();
    printf("[op_ret] %ld, %ld, %ld\n", ctx.rip, ctx.rbp, ctx.rsp);

    // if (ctx.rbp == 0)
    //     goto op_exit;
    // ctx.rsp = ctx.rbp;
    // ctx.rbp = ctx.pop<int32_t>();
    // ctx.rip = ctx.pop<int32_t>();
    // printf("[op_ret] %ld, %ld, %ld\n", ctx.rip, ctx.rsp, ctx.rbp);
}   goto DISPATCH;

op_exit:
    return ctx.rax;

op_CompileIR:
    printf("[op_CompileIR] rip=%ld\n", ctx.rip-1);
    return -1;

op_error:
    printf("invalid opcode %u\n", op->opcode);
    return -1;

    #undef BINARY_OP
    #undef UNARY_OP
    #undef DISPATCH
}

