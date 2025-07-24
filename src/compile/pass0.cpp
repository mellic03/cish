#include "./pass.hpp"
#include <stdio.h>

using namespace cish;


static int indent = 0;

template <typename... Args>
void iprintf( const char *fmt, Args... args )
{
    printf(fmt, args...);
    for (int i=0; i<indent; i++)
        printf(" ");
}


void CompilePass_0::visit_ErrorNode( CompileCtx &ctx, ErrorNode *N )
{
    Token *tok = N->m_state;
    printf("[PARSE ERROR line %lu, col %lu]", tok->lineno, tok->colno);
    iprintf(";\n");
}


void CompilePass_0::visit_NodeList( CompileCtx &ctx, NodeList *N )
{
    for (iNode *node: *N)
    {
        visit(ctx, node);
    }
}


void CompilePass_0::visit_DeclList( CompileCtx &ctx, DeclList *N )
{
    size_t count = 0;
    for (auto *decl: *N)
    {
        printf("%s %s", decl->m_type, decl->m_name);
        if (count++ < N->size()-1)
            printf(", ");
    }
}


void CompilePass_0::visit_CallList( CompileCtx &ctx, CallList *N )
{
    size_t count = 0;
    for (Idnt *idnt: *N)
    {
        printf("%s", idnt->m_name);
        if (count++ < N->size()-1)
            printf(", ");
    }
}


void CompilePass_0::visit_BlockScope( CompileCtx &ctx, BlockScope *N )
{
    indent += 4;
    iprintf("{\n");
    visit(ctx, N->m_body);
    indent -= 4;
    iprintf("\n");
    iprintf("}\n");
}


void CompilePass_0::visit_VarDecl( CompileCtx &ctx, VarDecl *N )
{
    printf("%s %s", N->m_type, N->m_name);
    iprintf(";\n");
}


void CompilePass_0::visit_FunDecl( CompileCtx &ctx, FunDecl *N )
{
    printf("%s %s(", N->m_type, N->m_name);
    visit(ctx, N->m_args);
    iprintf(")\n");
    visit(ctx, N->m_body);
    iprintf("\n");
}


void CompilePass_0::visit_RetNode( CompileCtx &ctx, RetNode *N )
{
    printf("return ");
    visit(ctx, N->m_expr);
    iprintf(";\n");
}


void CompilePass_0::visit_IfNode( CompileCtx &ctx, IfNode *N )
{

}


void CompilePass_0::visit_WhileNode( CompileCtx &ctx, WhileNode *N )
{

}


void CompilePass_0::visit_ForNode( CompileCtx &ctx, ForNode *N )
{

}



void CompilePass_0::visit_PrefixOp( CompileCtx &ctx, PrefixOp *N )
{

}


void CompilePass_0::visit_PostfixOp( CompileCtx &ctx, PostfixOp *N )
{

}


void CompilePass_0::visit_BinaryOp( CompileCtx &ctx, BinaryOp *N )
{
    printf("(");
    visit(ctx, N->m_lhs);
    printf(" %s ", N->m_opstr);
    visit(ctx, N->m_rhs);
    printf(")");

}


void CompilePass_0::visit_Assign( CompileCtx &ctx, Assign *N )
{
    printf("%s = ", N->m_idnt);
    visit(ctx, N->m_expr);
    iprintf(";\n");
}


void CompilePass_0::visit_FunCall( CompileCtx &ctx, FunCall *N )
{
    printf("%s(", N->m_name);
    visit(ctx, N->m_args);
    printf(")");
}



void CompilePass_0::visit_Idnt( CompileCtx &ctx, Idnt *N )
{
    printf("%s", N->m_name);
}


void CompilePass_0::visit_String( CompileCtx &ctx, String *N )
{
    printf("%s", N->m_str);
}


void CompilePass_0::visit_Number( CompileCtx &ctx, Number *N )
{
    printf("%s", N->m_lexeme);

}


