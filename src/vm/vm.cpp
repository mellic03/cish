#include "vm.hpp"
#include "ctx.hpp"
#include "bytecode.hpp"
#include <util/bitmanip.hpp>
#include <bit>
#include <stdio.h>
#include <string.h>

using namespace cish;



int cish::exec( uint32_t *program, size_t size )
{
    void *jtab[256] = {
        &&op_nop,

        &&op_vstor,
        &&op_vload,

        &&op_vpush32d,
        &&op_vpop32r,

        &&op_movrg, &&op_mov08, &&op_mov16, &&op_mov32,
        &&op_addrg, &&op_add08, &&op_add16, &&op_add32,
        &&op_subrg, &&op_sub08, &&op_sub16, &&op_sub32,
        &&op_mulrg, &&op_mul08, &&op_mul16, &&op_mul32,
        &&op_divrg, &&op_div08, &&op_div16, &&op_div32,

        &&op_push08r, &&op_push16r, &&op_push32r,
        &&op_push08d, &&op_push16d, &&op_push32d,
        &&op_pop08r,  &&op_pop16r,  &&op_pop32r,
        &&op_swap08,  &&op_swap16,  &&op_swap32,

        &&op_add, &&op_sub, &&op_mul, &&op_div,
        &&op_and, &&op_or,  &&op_xor, &&op_not,
        &&op_neg,

        &&op_equ, 
        &&op_les,  &&op_leq,  &&op_gtr, &&op_geq,
        &&op_jmp,  &&op_jeq,  &&op_jne,
        &&op_rjmp, &&op_rjeq, &&op_rjne,

        &&op_call,
        &&op_ret,

        &&op_exit,
    };

    for (int i=VmOp_NumOps; i<256; i++)
    {
        jtab[i] = &&op_error;
    }

    VmCtx ctx(program, size);
    auto *op = (VmOp*)(ctx.text);

    #define BINARY_OP(op_, tp_)  { ctx.push(ctx.pop<tp_>() op_ ctx.pop<tp_>()); }
    #define UNARY_OP(op_, tp_)   { ctx.push( op_ ctx.pop<tp_>() ); }

    #define BINARY_RD(op_, dx_)  { ctx.regs[op->regA] op_ op->dx_; }
    #define BINARY_RR(op_)       { ctx.regs[op->regA] op_ ctx.regs[op->regB]; }

    #define DISPATCH()\
        printf("[%lu] %lu\n", ctx.rip, op->opcode);\
        op = (ctx.text + ctx.rip++);\
        goto *jtab[op->opcode];

    DISPATCH();


op_nop: DISPATCH();

// op_stor32rr: ctx.stack[ctx.regs[op->regA]] = ctx.regs[op->regB];    DISPATCH();
// op_stor32rd: ctx.stack[ctx.regs[op->regA]] = op->d32;               DISPATCH();
// op_load32rr: ctx.regs[op->regA] = ctx.stack[ctx.regs[op->regB]];    DISPATCH();
// op_load32rd: ctx.regs[op->regA] = ctx.stack[op->d32];               DISPATCH();

op_vload: {
    printf("vload V[%lu]\n", ctx.vbp + op->d32);
    ctx.push( ctx.vstack[ctx.vbp + op->d32] );
}   DISPATCH();

op_vstor: {
    printf("vstor V[%lu]\n", ctx.vbp + op->d32);
    ctx.vstack[ctx.vbp + op->d32] = ctx.pop<uint32_t>();
}   DISPATCH();

op_vpush32d: ctx.vpush(op->d32);              DISPATCH();
op_vpop32r:  ctx.regs[op->regA] = ctx.vpop(); DISPATCH();

// op_vstor32rr: // mov [varstack + <A>], <B>
//     ctx.varstack[ctx.regs[op->regA]] = ctx.regs[op->regB];
//     DISPATCH();

// op_vstor32rd: // mov [varstack + <A>], d32
//     ctx.varstack[ctx.regs[op->regA]] = op->d32;
//     DISPATCH();

// op_vload32rr: // mov <A>, [varstack + <B>]
//     ctx.regs[op->regA] = ctx.varstack[ctx.regs[op->regB]];
//     DISPATCH();

// op_vload32rd: // mov <A>, [varstack + d32]
//     ctx.regs[op->regA] = ctx.varstack[op->d32];
//     DISPATCH();

// op_varpushrg: ctx.varpush(ctx.regs[op->regA]);  DISPATCH();
// op_varpush08: ctx.varpush(op->d08); DISPATCH();
// op_varpush16: ctx.varpush(op->d16); DISPATCH();
// op_varpush32: ctx.varpush(op->d32); DISPATCH();

// op_varpoprg:  ctx.regs[op->regA] = ctx.varpop<uint32_t>(); DISPATCH();
// op_varpop08:  ctx.varpop<uint8_t>();  DISPATCH();
// op_varpop16:  ctx.varpop<uint16_t>(); DISPATCH();
// op_varpop32:  ctx.varpop<uint32_t>(); DISPATCH();

op_movrg: BINARY_RR(=); DISPATCH();
op_mov08: BINARY_RD(=, d08); DISPATCH();
op_mov16: BINARY_RD(=, d16); DISPATCH();
op_mov32: BINARY_RD(=, d32); DISPATCH();

op_addrg: BINARY_RR(+=); DISPATCH();
op_add08: BINARY_RD(+=, d08); DISPATCH();
op_add16: BINARY_RD(+=, d16); DISPATCH();
op_add32: BINARY_RD(+=, d32); DISPATCH();

op_subrg: BINARY_RR(-=); DISPATCH();
op_sub08: BINARY_RD(-=, d08); DISPATCH();
op_sub16: BINARY_RD(-=, d16); DISPATCH();
op_sub32: BINARY_RD(-=, d32); DISPATCH();

op_mulrg: BINARY_RR(*=); DISPATCH();
op_mul08: BINARY_RD(*=, d08); DISPATCH();
op_mul16: BINARY_RD(*=, d16); DISPATCH();
op_mul32: BINARY_RD(*=, d32); DISPATCH();

op_divrg: BINARY_RR(/=); DISPATCH();
op_div08: BINARY_RD(/=, d08); DISPATCH();
op_div16: BINARY_RD(/=, d16); DISPATCH();
op_div32: BINARY_RD(/=, d32); DISPATCH();

op_push08r: ctx.push<uint8_t>(ctx.regs[op->regA]);    DISPATCH();
op_push16r: ctx.push<uint16_t>(ctx.regs[op->regA]);   DISPATCH();
op_push32r: ctx.push<uint32_t>(ctx.regs[op->regA]);   DISPATCH();

op_push08d: ctx.push<uint8_t>(op->d08);               DISPATCH();
op_push16d: ctx.push<uint16_t>(op->d16);              DISPATCH();
op_push32d: ctx.push<uint32_t>(op->d32);              DISPATCH();

op_pop08r:  ctx.regs[op->regA] = ctx.pop<uint8_t>();  DISPATCH();
op_pop16r:  ctx.regs[op->regA] = ctx.pop<uint16_t>(); DISPATCH();
op_pop32r:  ctx.regs[op->regA] = ctx.pop<uint32_t>(); DISPATCH();

// op_pushrg: ctx.push<uint32_t>(ctx.regs[op->regA]); DISPATCH();
// op_push08: ctx.push<uint8_t>(op->d08);  DISPATCH();
// op_push16: ctx.push<uint16_t>(op->d16); DISPATCH();
// op_push32: ctx.push<uint32_t>(op->d32); DISPATCH();

// op_poprg:  ctx.regs[op->regA] = ctx.pop<uint32_t>(); DISPATCH();
// op_pop08:  ctx.regs[op->regA] = ctx.pop<uint8_t>();  DISPATCH();
// op_pop16:  ctx.regs[op->regA] = ctx.pop<uint16_t>(); DISPATCH();
// op_pop32:  ctx.regs[op->regA] = ctx.pop<uint32_t>(); DISPATCH();

op_swap08: ctx.swap<uint8_t>();  DISPATCH();
op_swap16: ctx.swap<uint16_t>(); DISPATCH();
op_swap32: ctx.swap<uint32_t>(); DISPATCH();

op_add: BINARY_OP(+, uint32_t); DISPATCH();
op_sub: BINARY_OP(-, uint32_t); DISPATCH();
op_mul: BINARY_OP(*, uint32_t); DISPATCH();
op_div: BINARY_OP(/, uint32_t); DISPATCH();
op_and: BINARY_OP(&, uint32_t); DISPATCH();
op_or:  BINARY_OP(|, uint32_t); DISPATCH();
op_xor: BINARY_OP(^, uint32_t); DISPATCH();
op_not: UNARY_OP (~, uint32_t);  DISPATCH();
op_neg: UNARY_OP (-, uint32_t);  DISPATCH();

op_equ: BINARY_OP(==, uint32_t); DISPATCH();
op_les: BINARY_OP(< , uint32_t); DISPATCH();
op_leq: BINARY_OP(<=, uint32_t); DISPATCH();
op_gtr: BINARY_OP(> , uint32_t); DISPATCH();
op_geq: BINARY_OP(>=, uint32_t); DISPATCH();

op_jmp:
    // printf("[op_jmp] jmp %lu\n", op->d32);
    ctx.rip = op->d32;
    DISPATCH();

op_jeq:
    if (ctx.pop<uint32_t>() == ctx.pop<uint32_t>())
        ctx.jmp(op->d32);
    DISPATCH();

op_jne:
    if (ctx.pop<uint32_t>() != ctx.pop<uint32_t>())
        ctx.jmp(op->d32);
    DISPATCH();

op_rjmp:
    ctx.rjmp(op->d32);
    DISPATCH();

op_rjeq:
    if (ctx.pop<uint32_t>() == ctx.pop<uint32_t>())
        ctx.rjmp(op->d32);
    DISPATCH();

op_rjne:
    if (ctx.pop<uint32_t>() != ctx.pop<uint32_t>())
        ctx.rjmp(op->d32);
    DISPATCH();

op_call:
    // printf("[op_call] jmp %lu\n", op->d32);
    ctx.callstack.push({ctx.rip, ctx.rsp, ctx.rbp});
    ctx.rbp = ctx.rsp;
    ctx.rip = op->d32;
    DISPATCH();

op_ret: {
    if (ctx.rbp == 0)
        goto op_exit;
    auto [ip, sp, bp] = ctx.callstack.pop();
    ctx.rip = ip;
    ctx.rsp = sp;
    ctx.rbp = bp;
    // uint32_t ret = ctx.pop<uint32_t>();
    // printf("[op_ret] top=%lu\n", ret);
    // printf("[op_ret] rip=%lu, rsp=%lu\n", ctx.rip, ctx.rsp);
}   DISPATCH();

op_exit:
    return ctx.pop<uint32_t>();

op_error:
    printf("[op_error] op=%u\n", op->opcode);
    return -1;

    #undef BINARY_OP
    #undef UNARY_OP
    #undef BINARY_RD
    #undef BINARY_RR
    #undef BINARY_RD2
    #undef BINARY_RR2
    #undef DISPATCH
}

