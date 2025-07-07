#include "ctx.hpp"
#include "bytecode.hpp"


cish::VmCtx::VmCtx( uint32_t *program, size_t size )
:   text((VmOp*)program),
    stack((uint8_t*)(new uint32_t[512]))
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

