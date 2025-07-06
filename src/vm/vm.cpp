#include "vm.hpp"
#include "bytecode.hpp"
#include <bit>
#include <stdio.h>
#include <string.h>

using namespace cish;



cish::VmCtx::VmCtx( uint32_t *program, size_t size )
:   txt((VmOp*)program),
    rip(0),
    // rbp(size-1),
    stack(new uint64_t[256]),
    rsp(0)
{
    memset(regs, 0, sizeof(regs));
    // ((VmOp*)&txt[rbp])->opcode = VmOp_exit;
}



static uint64_t packU64( const VmOpU64 &data )
{
    VmOpU64LoHi U;
    U.lo = data.lo;
    U.hi = data.hi;
    return U.u64;
}



int cish::exec( uint32_t *program, size_t size )
{
    void *jtab[256] = {
        &&op_nop,

        &&op_movrd, &&op_movrr, &&op_movrrd, &&op_movrdr,
        &&op_pushd, &&op_pop,   &&op_pushr,  &&op_popr,
        &&op_pushrd,

        &&op_addrd, &&op_subrd, &&op_mulrd,  &&op_divrd,
        &&op_andrd, &&op_orrd,  &&op_xorrd,

        &&op_addrr, &&op_subrr, &&op_mulrr,  &&op_divrr,
        &&op_andrr, &&op_orrr,  &&op_xorrr,

        &&op_adds,  &&op_subs,  &&op_muls,   &&op_divs,
        &&op_ands,  &&op_ors,   &&op_xors,

        &&op_jmpd,  &&op_jmpr,  &&op_rjmpd,  &&op_rjmpr,
        &&op_calld, &&op_callr, &&op_ret,

        &&op_exit,
    };

    for (int i=VmOp_NumOps; i<256; i++)
    {
        jtab[i] = &&op_error;
    }

    VmCtx ctx(program, size);

    auto *op    = (VmOp*)(ctx.txt - 1);
    auto *opd   = (VmOpData*)op;
    auto *opr   = (VmOpReg*)op;
    auto *oprd  = (VmOpRegData*)op;
    auto *oprr  = (VmOpRegReg*)op;
    auto *oprrd = (VmOpRegRegData*)op;
    auto *oprdr = (VmOpRegDataReg*)op;

    #define BINARY_OP(op_) { auto A=ctx.pop(); auto B=ctx.pop(); ctx.push(B op_ A); }
    #define UNARY_OP(op_)  { ctx.push(op_ ctx.pop()); }
    #define BINARY_RD(op_) { ctx.regs[oprd->idx] op_ packU64(oprd->data); }
    #define BINARY_RR(op_) { ctx.regs[oprr->idxA] op_ ctx.regs[oprr->idxB]; }

    #define DISPATCH()\
        op   += 1;\
        opd   = (VmOpData*)op;\
        opr   = (VmOpReg*)op;\
        oprd  = (VmOpRegData*)op;\
        oprr  = (VmOpRegReg*)op;\
        oprrd = (VmOpRegRegData*)op;\
        oprdr = (VmOpRegDataReg*)op;\
        goto *jtab[op->opcode];

    #define DISPATCH_BIGOP(op_)\
        op    = (VmOp*)(op_->next);\
        opd   = (VmOpData*)op;\
        opr   = (VmOpReg*)op;\
        oprd  = (VmOpRegData*)op;\
        oprr  = (VmOpRegReg*)op;\
        oprrd = (VmOpRegRegData*)op;\
        oprdr = (VmOpRegDataReg*)op;\
        goto *jtab[op->opcode];

    DISPATCH();


op_nop:     DISPATCH();
op_movrd: { BINARY_RD(=) DISPATCH_BIGOP(oprd); }
op_movrr: { BINARY_RR(=) DISPATCH_BIGOP(oprr); }

op_movrrd: {
    auto off = ctx.regs[oprrd->idxB] + packU64(oprrd->data);
    ctx.regs[oprrd->idxA] = ctx.stack[off];
}   DISPATCH_BIGOP(oprrd);

op_movrdr: {
    auto off = ctx.regs[oprdr->idxA] + packU64(oprdr->data);
    ctx.stack[off] = ctx.regs[oprdr->idxB];
}   DISPATCH_BIGOP(oprdr);

op_pushd: {
    ctx.push(packU64(opd->data));
}   DISPATCH_BIGOP(opd);

op_pop: {
    ctx.pop();
}   DISPATCH();

op_pushr: {
    ctx.push(ctx.regs[opr->idx]);
}   DISPATCH_BIGOP(opr);

op_popr: {
    ctx.regs[opr->idx] = ctx.pop();
}   DISPATCH_BIGOP(opr);

op_pushrd: {
    auto off = ctx.regs[oprd->idx] + packU64(oprd->data);
    ctx.push(ctx.stack[off]);
}   DISPATCH_BIGOP(oprd);

op_addrd: { BINARY_RD(+=); DISPATCH_BIGOP(oprd); }
op_subrd: { BINARY_RD(-=); DISPATCH_BIGOP(oprd); }
op_mulrd: { BINARY_RD(*=); DISPATCH_BIGOP(oprd); }
op_divrd: { BINARY_RD(/=); DISPATCH_BIGOP(oprd); }
op_andrd: { BINARY_RD(&) DISPATCH_BIGOP(oprd); }
op_orrd:  { BINARY_RD(|) DISPATCH_BIGOP(oprd); }
op_xorrd: { BINARY_RD(^) DISPATCH_BIGOP(oprd); }

op_addrr: { BINARY_RR(+=); DISPATCH_BIGOP(oprr); }
op_subrr: { BINARY_RR(-=); DISPATCH_BIGOP(oprr); }
op_mulrr: { BINARY_RR(*=); DISPATCH_BIGOP(oprr); }
op_divrr: { BINARY_RR(/=); DISPATCH_BIGOP(oprr); }
op_andrr: { BINARY_RR(&) DISPATCH_BIGOP(oprr); }
op_orrr:  { BINARY_RR(|) DISPATCH_BIGOP(oprr); }
op_xorrr: { BINARY_RR(^) DISPATCH_BIGOP(oprr); }

op_adds: { BINARY_OP(+); DISPATCH(); }
op_subs: { BINARY_OP(-); DISPATCH(); }
op_muls: { BINARY_OP(*); DISPATCH(); }
op_divs: { BINARY_OP(/); DISPATCH(); }
op_ands: { BINARY_OP(&); DISPATCH(); }
op_ors:  { BINARY_OP(|); DISPATCH(); }
op_xors: { BINARY_OP(^); DISPATCH(); }

op_jmpd: {
    ctx.rip = packU64(opd->data);
}   DISPATCH();

op_jmpr: {
    ctx.rip = ctx.regs[opr->idx];
}   DISPATCH();

op_rjmpd: {
    ctx.rip += packU64(opd->data);
}   DISPATCH();

op_rjmpr: {
    ctx.rip += ctx.regs[opr->idx];
}   DISPATCH();

op_calld: {
    ctx.push(ctx.rbp);
    ctx.rbp = ctx.rsp;
    ctx.rip = packU64(opd->data);
}   DISPATCH();

op_callr: {
    ctx.push(ctx.rbp);
    ctx.rbp = ctx.rsp;
    ctx.rip = ctx.regs[opr->idx];
}   DISPATCH();

op_ret: {
    if (ctx.rbp == 0)
        goto op_exit;
    ctx.rsp = ctx.rbp;
    ctx.rbp = ctx.pop();
}   DISPATCH();

op_exit:
    return ctx.top();

// op_print: {
//     uint64_t argc = ctx.pop();
//     for (uint64_t i=0; i<argc; i++)
//         printf("%lu ", ctx.pop());
// }   DISPATCH();

op_error:
    printf("[op_error] op=%u\n", op->opcode);
    return -1;

    #undef DISPATCH
    #undef BINARY_OP
    #undef UNARY_OP
}

