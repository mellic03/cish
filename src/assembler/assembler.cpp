#include "assembler.hpp"
#include "../vm/ctx.hpp"
#include "../vm/bytecode.hpp"
#include "../vm/vm.hpp"
#include <stdio.h>

static const char *VmRegStr( uint8_t );
const char *VmOpStr( uint8_t );


void cish::disassemble( void *base, size_t size )
{
    size_t rip = 0;
    VmOp  *buf = (VmOp*)base;
    VmOp  *op  = buf + rip;

    while (op)
    {
        printf("[%lu] %s", rip, VmOpStr(op->opcode));
    
        if (op->dstreg)
            printf(" %s", VmRegStr(op->dstreg));
        if (op->srcreg)
            printf(" %s", VmRegStr(op->srcreg));
        printf(" %u\n", op->d32);

        if (op->opcode == VmOp_ret)
            printf("\n");

        if (op->opcode == VmOp_exit)
            break;
        op = buf + rip++;
    }
}



#include "../vm/bytecode.hpp"

static const char *VmRegStr( uint8_t idx )
{
    using namespace cish;

    switch (idx)
    {
        default:       return "INVALID";
        case Reg_rnul: return "nul";
        case Reg_rax:  return "rax";
        case Reg_rbx:  return "rbx";
        case Reg_rcx:  return "rcx";
        case Reg_rdx:  return "rdx";
        case Reg_rsi:  return "rsi";
        case Reg_rdi:  return "rdi";
        case Reg_rsp:  return "rsp";
        case Reg_rbp:  return "rbp";
        case Reg_rip:  return "rip";
        case Reg_memptr:    return "memptr";
        case Reg_rspoff:    return "rspoff";
        case Reg_rbpoff:    return "rbpoff";
        case Reg_rcmp0:     return "rcmp0";
        case Reg_rcmp1:     return "rcmp1";
        case Reg_tmp:       return "tmp";
    }
}


const char *VmOpStr( uint8_t opcode )
{
    using namespace cish;

    switch (opcode)
    {
        default:        return "INVALID";
        case VmOp_nop:  return "nop";
        case VmOp_mov_rxi:  return "mov_rxi";
        case VmOp_mov_rrx:  return "mov_rrx";
        case VmOp_gload_xi: return "gload_xi";
        case VmOp_gstor_ix: return "gstor_ix";
        case VmOp_vload_xi: return "vload_xi";
        case VmOp_vstor_ix: return "vstor_ix";
        case VmOp_add_rxi:   return "add_rxi";
        case VmOp_add_rrx:   return "add_rrx";
        case VmOp_push_xxi: return "push_xxi";
        case VmOp_push_xrx: return "push_xrx";
        case VmOp_pop_rxx:  return "pop_rxx";
        case VmOp_swap: return "swap";
        case VmOp_add:  return "add";
        case VmOp_sub:  return "sub";
        case VmOp_mul:  return "mul";
        case VmOp_div:  return "div";
        case VmOp_and:  return "and";
        case VmOp_or:   return "or";
        case VmOp_xor:  return "xor";
        case VmOp_not:  return "not";
        case VmOp_neg:  return "neg";
        case VmOp_equ:  return "equ";
        case VmOp_les:  return "les";
        case VmOp_leq:  return "leq";
        case VmOp_gtr:  return "gtr";
        case VmOp_geq:  return "geq";
        case VmOp_jmp:  return "jmp";
        case VmOp_jeq:  return "jeq";
        case VmOp_jne:  return "jne";
        case VmOp_call: return "call";
        case VmOp_ret:  return "ret";
        case VmOp_exit: return "exit";
        case VmOp_CompileIR:    return "CompileIR";
    }
}

