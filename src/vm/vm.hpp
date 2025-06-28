#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    union VmOp;

    struct VmCtx
    {
        uint8_t  *rip;
        uint64_t *rsp;
        uint64_t  rbp;
        uint64_t  regs[16];

        VmCtx( uint64_t *s );
        VmCtx( const VmCtx& ) = delete;
        VmCtx( VmCtx&& ) = delete;

        uint8_t *next();

        uint64_t *getDst();
        uint64_t  getSrc();
    };

    void exec( uint8_t* );
}

