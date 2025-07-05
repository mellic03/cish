#pragma once

#include <stddef.h>
#include <stdint.h>

struct AstNode;

namespace cish
{
    uint8_t *assemble( AstNode *ast, uint64_t *buf, size_t bufsz );

}


// struct iAstNode;

// class cish::Assembler
// {
//     void assemble( iAstNode* );
// };