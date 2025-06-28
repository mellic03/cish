#pragma once
#include <stddef.h>
#include <stdint.h>

namespace cish
{
    struct VmCtx
    {
        uint8_t  *rip;
        uint64_t *rsp;
        uint64_t regs[16];
    };

}
