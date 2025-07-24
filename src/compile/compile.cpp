#include "../vm/bytecode.hpp"
#include "../vm/ctx.hpp"
#include "../vm/vm.hpp"

#include <cish/compile.hpp>
#include <cish/compile-ctx.hpp>
#include <cish/parse-node.hpp>
#include "./pass.hpp"

#include <stdio.h>


using namespace cish;


uint64_t *cish::compile( CompileCtx &ctx, iNode *root )
{
    CompilePass_0().visit(ctx, root);

    CompilePass_1().visit(ctx, root);

    CompilePass_2().visit(ctx, root);

    return nullptr;
}

