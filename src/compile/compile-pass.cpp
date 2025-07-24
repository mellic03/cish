#include <cish/compile-pass.hpp>
#include <assert.h>

using namespace cish;


void cish::NodeVisitor::visit( CompileCtx &ctx, iNode *N )
{
    switch (N->tag)
    {
        default: assert((false)); return;

        #define X(name) case ParseTag::name: visit_ ##name(ctx, (name*)N); return;
        PARSE_TAGS
        #undef X
    }

    assert((false));
}



