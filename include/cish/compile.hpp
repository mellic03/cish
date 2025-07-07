#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    struct AstNode;
    uint32_t *compile( AstNode *ast, uint32_t *buf, size_t bufsz );
}

