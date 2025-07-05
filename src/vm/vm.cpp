#include "vm.hpp"
#include "bytecode.hpp"
#include <bit>
#include <stdio.h>

using namespace cish;



cish::VmCtx::VmCtx( uint64_t *program, size_t size )
:   txt((VmOp*)program),
    rip(0),
    // rbp(size-1),
    stack(new uint64_t[256]),
    rsp(0)
{
    // ((VmOp*)&txt[rbp])->opcode = VmOp_exit;
}


int cish::exec( uint64_t *base, size_t size )
{
    void *jtab[256] = {
        &&op_error, 
        &&op_push,  &&op_pop,
        &&op_pushn, &&op_popn,
        &&op_addu,  &&op_subu, &&op_mulu, &&op_divu,
        &&op_addi,  &&op_subi, &&op_muli, &&op_divi,
        &&op_and,   &&op_or,   &&op_xor,
        &&op_neg,   &&op_not,
        &&op_jmp,   &&op_rjmp,
        &&op_call,  &&op_ret,
        &&op_exit,
        &&op_print,
    };

    for (int i=VmOp_NumOps; i<256; i++)
    {
        jtab[i] = &&op_error;
    }

    VmCtx ctx(base, size);
    VmOp *op = ctx.txt - 1;

    #define DISPATCH()\
        op++;\
        ctx.rip = op - ctx.txt;\
        goto *jtab[uint8_t(op->opcode)];

    #define BINARY_OP(op_)\
        ctx.push(ctx.pop() op_ ctx.pop());

    #define UNARY_OP(op_)\
        ctx.push(op_ ctx.pop());

    DISPATCH();

op_push: ctx.push(op->data); DISPATCH();
op_pop:  ctx.pop();          DISPATCH();

op_pushn: ctx.rsp += op->data; DISPATCH();
op_popn:  ctx.rsp -= op->data; DISPATCH();

op_addu: op_addi: BINARY_OP(+); DISPATCH();
op_subu: op_subi: BINARY_OP(-); DISPATCH();
op_mulu: op_muli: BINARY_OP(*); DISPATCH();
op_divu: op_divi: BINARY_OP(/); DISPATCH();

op_and: BINARY_OP(&); DISPATCH();
op_or:  BINARY_OP(|); DISPATCH();
op_xor: BINARY_OP(^); DISPATCH();

op_neg: UNARY_OP(-);  DISPATCH();
op_not: UNARY_OP(!);  DISPATCH();

op_jmp:  ctx.rip  = ctx.pop(); DISPATCH();
op_rjmp: ctx.rip += ctx.pop(); DISPATCH();

op_call: {
    // uint64_t x = ctx.pop();
    // ctx.push(ctx.rbp);
    // ctx.push(ctx.rip);
    // ctx.rbp = ctx.rsp;
    // ctx.jmp(x);
}   DISPATCH();

op_ret:
    // ctx.rsp = ctx.rbp;
    // ctx.rip = ctx.pop();
    // ctx.rbp = ctx.pop();
    // DISPATCH();

op_exit:
    return ctx.top();

op_print: {
    uint64_t argc = ctx.pop();
    for (uint64_t i=0; i<argc; i++)
        printf("%lu ", ctx.pop());
}   DISPATCH();

op_error:
    return -1;

    #undef DISPATCH
    #undef BINARY_OP
    #undef UNARY_OP
}





// uint8_t *cish::VmCtx::next()
// {
//     VmOp     *op  = (VmOp*)rip;
//     uint64_t *dst = getDst();
//     uint64_t  src = getSrc();

//     switch (op->opcode)
//     {
//         case VmOp_mov: *dst  = src; break;
//         case VmOp_add: *dst += src; break;
//         case VmOp_sub: *dst -= src; break;
//         case VmOp_mul: *dst *= src; break;
//         case VmOp_div: *dst /= src; break;
//         case VmOp_and: *dst &= src; break;
//         case VmOp_or:  *dst |= src; break;
//         case VmOp_xor: *dst ^= src; break;

//         case VmOp_push: *(rsp++) = src;  break;
//         case VmOp_pop:  *dst = *(--rsp); break;

//         case VmOp_jmp:  rip = (uint8_t*)src; break;

//         case VmOp_call:
//             *(rsp++) = ((uint64_t)rip)+1; // push rip
//             rbp = (uint64_t)rsp;          // save rsp
//             rip = (uint8_t*)src;          // jmp
//             break;

//         case VmOp_ret:
//             rsp = (uint64_t*)rbp;         // load rsp
//             rip = (uint8_t*)(*(--rsp));   // pop rip
//             break;

//         case VmOp_exit: rip = nullptr; break;
//     }

//     return (uint8_t*)op;
// }




// cish::VmCtx::VmCtx( uint64_t *s )
// :   rsp(s)
// {
    
// }


// uint64_t *cish::VmCtx::getDst()
// {
//     VmOp *op     = (VmOp*)rip;
//     auto *as_u8  = (uint8_t*)rip;
//     auto *as_u64 = (uint64_t*)rip;
 
//     if (op->dstIsReg)
//     {
//         uint8_t regIdx = as_u8[1];
//         return &regs[regIdx];
//     }

//     else
//     {
//         uint64_t addr = as_u64[1];
//         return (uint64_t*)addr;
//     }
// }


// uint64_t cish::VmCtx::getSrc()
// {
//     VmOp *op     = (VmOp*)rip;
//     auto *as_u8  = (uint8_t*)rip;
//     auto *as_u64 = (uint64_t*)rip;

//     if (op->srcIsReg)
//     {
//         uint8_t regIdx = as_u8[1];
//         return regs[regIdx];
//     }

//     else
//     {
//         return as_u64[1];
//     }
// }

    