#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    struct VmOp;

    int exec( VmOp*, size_t );
}

