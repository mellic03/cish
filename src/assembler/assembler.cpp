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
        case Reg_vsp:       return "vsp";
        case Reg_vbp:       return "vbp";
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
        default:            return "INVALID";
        case VmOp_nop:      return "nop";
        case OpRI_mov:      return "ri_mov";
        case OpRR_mov:      return "rr_mov";
        case OpRI_add:      return "ri_add";
        case OpRR_add:      return "rr_add";
        case OpXI_gload:    return "xi_gload";
        case OpIX_gstor:    return "ix_gstor";
        case OpXI_vload:    return "xi_vload";
        case OpIX_vstor:    return "ix_vstor";
        case OpI_push:      return "push_xxi";
        case OpR_push:      return "push_xrx";
        case OpR_pop:       return "pop_rxx";
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

