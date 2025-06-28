#include "assembler.hpp"
#include "../ast.hpp"


// char cish::global_strab[1024];
// char *strtab_top = cish::global_strab;


// static
// const char *strtab_write( const char *str )
// {
//     auto *dst = strtab_top;
//     size_t size = strlen(str) + 1;
//     strtab_top += size;
//     memcpy(dst, str, size);
//     return dst;
// }

using namespace cish;


void Assembler::assemble( iAstNode *AST )
{

}