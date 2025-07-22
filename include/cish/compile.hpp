#pragma once
#include <stddef.h>
#include <stdint.h>

namespace cish
{
    class CompileCtx;
    struct AstNode;
    uint64_t *compile( CompileCtx&, AstNode* );
}

