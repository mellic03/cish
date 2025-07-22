#include "../vm/bytecode.hpp"
#include "../vm/ctx.hpp"
#include "../vm/vm.hpp"

#include <cish/compile.hpp>
#include <cish/compile-ctx.hpp>
#include <cish/parse-node.hpp>
#include "./pass1.hpp"

#include <assert.h>

using namespace cish;

// extern void compile_pass1( CompileCtx&, AstNode* );
// extern void compile_pass2( CompileCtx&, AstNode* );

uint64_t *cish::compile( CompileCtx &ctx, ProdNode *node )
{
    // CompilePass_1 pass1;
    // pass1.visit(ctx, *node);

    // compile_pass1(ctx, ast);
    // compile_pass2(ctx, ast);

    return nullptr;
}

