#pragma once
#include <stddef.h>
#include <stdint.h>

namespace cish
{
    enum VmOp_: uint8_t
    {
        VmOp_Exit,

        VmOp_Mov,

        VmOp_Add,
        VmOp_Sub,
        VmOp_Mul,
        VmOp_Div,

        VmOp_LAnd,
        VmOp_LOr,
        VmOp_LXor,
        VmOp_LNot,

        VmOp_Push, VmOp_Pop,
        VmOp_Call, VmOp_Ret,

        VmOp_NumOps,
    };

    struct VmByteOp
    {
        uint8_t op :7;
        uint8_t sz :1;
    };

    struct VmWordOp
    {
        uint16_t op :15;
        uint16_t sz :1;
    };

}
