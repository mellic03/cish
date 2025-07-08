#include "vm.hpp"
#include "ctx.hpp"
#include "bytecode.hpp"
#include <util/bitmanip.hpp>
#include <bit>
#include <stdio.h>
#include <string.h>

using namespace cish;


extern const char *VmOpStr( uint8_t opcode );

int cish::exec( uint32_t *program, size_t size )
{
    void *jtab[256] = {
        &&op_nop,

        &&op_ri_mov,
        &&op_rr_mov,
        &&op_ri_add,
        &&op_rr_add,

        &&op_i_gload,
        &&op_i_gstor,
        &&op_i_vload,
        &&op_i_vstor,

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

    VmCtx ctx(program, size);
    auto *op = (VmOp*)(ctx.text);

    #define DST_REG                ctx.regs[op->dstreg]
    #define SRC_REG                ctx.regs[op->srcreg]
    #define BINARY_OP(op_)  { ctx.push(ctx.pop() op_ ctx.pop()); }
    #define UNARY_OP(op_)   { ctx.push( op_ ctx.pop() ); }
    #define STACK32(Idx)           ctx.stack[Idx]

    #define DISPATCH() op=&ctx.text[ctx.rip++];  goto *jtab[op->opcode];
    // #define DISPATCH() 
    // op=(ctx.text + ctx.rip++);
        // printf("[%lu]\t", ctx.rip);
    // for (int i=ctx.rsp-1; i>=0; i--)
        // printf("%lu ", ctx.stack[i]);
    // printf("\n");
    // printf("[%lu]\t%s [%u %u %u]\n\n", ctx.rip, VmOpStr(op->opcode), op->dstreg, op->srcreg, op->imm);
    // goto *jtab[op->opcode];

    DISPATCH();


op_nop: DISPATCH();

op_ri_mov: DST_REG = op->imm; DISPATCH();
op_rr_mov: DST_REG = SRC_REG; DISPATCH();
op_ri_add: DST_REG += op->imm; DISPATCH();
op_rr_add: DST_REG += SRC_REG; DISPATCH();

op_i_gload:
    ctx.push(ctx.vstack[op->imm]);
    DISPATCH();

op_i_gstor:
    ctx.vstack[op->imm] = ctx.pop();
    DISPATCH();

op_i_vload:
    ctx.push(ctx.vstack[ctx.vbp + op->imm]);
    DISPATCH();

op_i_vstor:
    ctx.vstack[ctx.vbp + op->imm] = ctx.pop();
    DISPATCH();

op_i_push: ctx.push(op->imm);   DISPATCH();
op_r_push: ctx.push(SRC_REG);   DISPATCH();
op_r_pop:  DST_REG = ctx.pop(); DISPATCH();
op_swap:   ctx.swap();          DISPATCH();

op_add:  BINARY_OP(+);  DISPATCH();
op_sub:  BINARY_OP(-);  DISPATCH();
op_mul:  BINARY_OP(*);  DISPATCH();
op_div:  BINARY_OP(/);  DISPATCH();
op_and:  BINARY_OP(&);  DISPATCH();
op_or:   BINARY_OP(|);  DISPATCH();
op_xor:  BINARY_OP(^);  DISPATCH();
op_not:  UNARY_OP (~);  DISPATCH();
op_neg:  UNARY_OP (-);  DISPATCH();
op_equ:  BINARY_OP(==); DISPATCH();
op_les:  BINARY_OP(<);  DISPATCH();
op_leq:  BINARY_OP(<);  DISPATCH();
op_gtr:  BINARY_OP(>);  DISPATCH();
op_geq:  BINARY_OP(>);  DISPATCH();

op_jmp: ctx.rip = op->imm;                          DISPATCH();
op_jeq: if (ctx.rcmp0==ctx.rcmp1) ctx.rip=op->imm;  DISPATCH();
op_jne: if (ctx.rcmp0!=ctx.rcmp1) ctx.rip=op->imm;  DISPATCH();

op_call: {
    ctx.callpush();
    ctx.rip = op->imm;
}   DISPATCH();

op_ret: {
    if (ctx.callstack.size() == 0)
        goto op_exit;
    ctx.callpop();
}   DISPATCH();

op_exit:
    return ctx.rax;

op_CompileIR:
    printf("[op_CompileIR] rip=%lu\n", ctx.rip-1);
    return -1;

op_error:
    printf("invalid opcode %u\n", op->opcode);
    return -1;

    #undef BINARY_OP
    #undef UNARY_OP
    #undef DISPATCH
}

