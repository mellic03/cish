#include "vm.hpp"
#include "bytecode.hpp"


using namespace cish;


void cish::exec( uint8_t *base )
{
    VmCtx ctx(new uint64_t[128]);
    ctx.rip = base;

    while (ctx.rip)
    {
        ctx.next();
        ctx.rip++;
    }

}






uint8_t *cish::VmCtx::next()
{
    VmOp     *op  = (VmOp*)rip;
    uint64_t *dst = getDst();
    uint64_t  src = getSrc();

    switch (op->opcode)
    {
        case VmOp_mov: *dst  = src; break;
        case VmOp_add: *dst += src; break;
        case VmOp_sub: *dst -= src; break;
        case VmOp_mul: *dst *= src; break;
        case VmOp_div: *dst /= src; break;
        case VmOp_and: *dst &= src; break;
        case VmOp_or:  *dst |= src; break;
        case VmOp_xor: *dst ^= src; break;

        case VmOp_push: *(rsp++) = src;  break;
        case VmOp_pop:  *dst = *(--rsp); break;

        case VmOp_jmp:  rip = (uint8_t*)src; break;

        case VmOp_call:
            *(rsp++) = ((uint64_t)rip)+1; // push rip
            rbp = (uint64_t)rsp;          // save rsp
            rip = (uint8_t*)src;          // jmp
            break;

        case VmOp_ret:
            rsp = (uint64_t*)rbp;         // load rsp
            rip = (uint8_t*)(*(--rsp));   // pop rip
            break;

        case VmOp_exit: rip = nullptr; break;
    }

    return (uint8_t*)op;
}




cish::VmCtx::VmCtx( uint64_t *s )
:   rsp(s)
{
    
}


uint64_t *cish::VmCtx::getDst()
{
    VmOp *op     = (VmOp*)rip;
    auto *as_u8  = (uint8_t*)rip;
    auto *as_u64 = (uint64_t*)rip;
 
    if (op->dstIsReg)
    {
        uint8_t regIdx = as_u8[1];
        return &regs[regIdx];
    }

    else
    {
        uint64_t addr = as_u64[1];
        return (uint64_t*)addr;
    }
}


uint64_t cish::VmCtx::getSrc()
{
    VmOp *op     = (VmOp*)rip;
    auto *as_u8  = (uint8_t*)rip;
    auto *as_u64 = (uint64_t*)rip;

    if (op->srcIsReg)
    {
        uint8_t regIdx = as_u8[1];
        return regs[regIdx];
    }

    else
    {
        return as_u64[1];
    }
}

    