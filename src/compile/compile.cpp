#include "../vm/bytecode.hpp"
#include "../vm/ctx.hpp"
#include "../vm/vm.hpp"
#include <cish/compile.hpp>
#include <cish/compile-ctx.hpp>
#include <cish/node.hpp>
#include <assert.h>

using namespace cish;

extern void compile_pass1( CompileCtx&, AstNode* );
extern void compile_pass2( CompileCtx&, AstNode* );


uint32_t *cish::compile( AstNode *ast, uint32_t *buf, size_t bufsz )
{
    CompileCtx ctx(buf, bufsz);

    compile_pass1(ctx, ast);
    compile_pass2(ctx, ast);

    return nullptr;
}

