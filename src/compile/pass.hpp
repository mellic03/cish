#pragma once
#include <cish/compile-pass.hpp>

namespace cish
{
    struct CompilePass_0: public NodeVisitor
    {
        #define X(name) virtual void visit_##name( CompileCtx&, name* ) final;
        PARSE_TAGS
        #undef X
    };


    // Add definitions to symbol table.
    struct CompilePass_1: public NodeVisitor
    {
        #define X(name) virtual void visit_##name( CompileCtx&, name* ) final;
        PARSE_TAGS
        #undef X
    };


    // Resolve information from populated symbol table.
    struct CompilePass_2: public NodeVisitor
    {
        #define X(name) virtual void visit_##name( CompileCtx&, name* ) final;
        PARSE_TAGS
        #undef X
    };
}

