#include "assembler.hpp"
#include "../vm/ctx.hpp"
#include "../vm/bytecode.hpp"
#include "../vm/vm.hpp"
#include <stdio.h>

const char *VmRegStr( uint8_t );
const char *VmOpStr( uint8_t );


void cish::disassemble( void *base, size_t size )
{
    size_t  rip = 0;
    VmOp   *buf = (VmOp*)base;
    VmOp   *op  = buf;
    VmOp   *end = buf + size;

    while (op < end)
    {
        printf("[%lu] %s", rip, VmOpStr(op->opcode));
        if (op->dstreg) printf(" %s", VmRegStr(op->dstreg));
        if (op->srcreg) printf(" %s", VmRegStr(op->srcreg));
        printf(" %d\n", op->imm);

        if (op->opcode == VmOp_ret)
            printf("\n");

        if (op->opcode == VmOp_exit)
            break;
        
        if (op->opcode >= VmOp_NumOps)
            break;

        op = (buf + rip++);
    }
}



#include "../vm/bytecode.hpp"

const char *VmRegStr( uint8_t idx )
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
        case OpI_prnt:      return "i_prnt";
        case OpR_prnt:      return "r_prnt";
        
        case OpRI_mov:      return "RI_mov";
        case OpRI_add:      return "RI_add";
        case OpRI_sub:      return "RI_sub";
        case OpRI_mul:      return "RI_mul";
        case OpRI_div:      return "RI_div";
        case OpRR_mov:      return "RR_mov";
        case OpRR_add:      return "RR_add";
        case OpRR_sub:      return "RR_sub";
        case OpRR_mul:      return "RR_mul";
        case OpRR_div:      return "RR_div";

        case OpI_gload:     return "i_gload";
        case OpI_gstor:     return "i_gstor";
        case OpI_lload:     return "i_vload";
        case OpI_lstor:     return "i_lstor";
        case OpI_push:      return "i_push";
        case OpR_push:      return "r_push";
        case OpR_pop:       return "r_pop";
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

