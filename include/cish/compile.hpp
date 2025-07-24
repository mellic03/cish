#pragma once
#include <stddef.h>
#include <stdint.h>

namespace cish
{
    class CompileCtx;
    struct iNode;

    // uint64_t *compile( CompileCtx&, AstNode* );
    uint64_t *compile( CompileCtx&, iNode* );
}

