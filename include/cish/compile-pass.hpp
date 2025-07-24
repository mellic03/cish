#pragma once

#include <cish/parse-node.hpp>


namespace cish
{
    class CompileCtx;

    struct NodeVisitor
    {
        void visit( CompileCtx&, iNode *N );

        #define X(name) virtual void visit_##name( CompileCtx&, name* ) = 0;
        PARSE_TAGS
        #undef X
    };
}

