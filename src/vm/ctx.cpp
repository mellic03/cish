#include "ctx.hpp"
#include "bytecode.hpp"


cish::VmCtx::VmCtx( uint32_t *program, size_t size )
:   text((VmOp*)program),
    stack(new uint32_t[512]),
    vstack(new uint32_t[512])
{
    memset(regs, 0, sizeof(regs));
    // ((VmOp*)&txt[rbp])->opcode = VmOp_exit;
}


void cish::VmCtx::jmp( uint32_t addr )
{
    rip = addr;
}


void cish::VmCtx::rjmp( uint32_t addr )
{
    rip += addr;
}



void cish::VmCtx::callpush()
{
    callstack.push({rip, rsp, rbp, vsp, vbp});
    rbp = align_up(rsp, 16);
    vbp = align_up(vsp, 16);
}


void cish::VmCtx::callpop()
{
    auto [ip, sp, bp, v_sp, v_bp] = callstack.pop();
    rip=ip;
    rsp=sp; rbp=bp;
    vsp=v_sp; vbp=v_bp;
}


