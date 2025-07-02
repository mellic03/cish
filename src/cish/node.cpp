#include <cish/node.hpp>
#include <cish/symtab.hpp>
#include <cish/compile.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

using namespace cish;


NodeError::NodeError( const char *fmt, ... )
:   Node(Type::Error)
{
    va_list vlist;
    va_start(vlist, fmt);
    fprintf(stderr, "[PARSE ERROR] ");
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);
};



void NodeGrouping::print()
{
    // printf("Grouping[");

    for (auto *N: m_nodes)
    {
        N->print();
        if (N->m_type != Type::Grouping)
            printf("\n");
    }
    printf("\n");
}

void NodeProgRoot::print()
{
    NodeGrouping::print();
}

void NodeBinary::print()
{
    printf("(");
    m_lhs->print();
    printf(" %s ", TypeToStr(m_type));
    m_rhs->print();
    printf(")");
}

void NodeUnary::print()
{
    printf("(%s", TypeToStr(m_type));
    m_rhs->print();
    printf(")");
}


// void NodeDecl::print()
// {
//     printf("decl(");
//     printf(m_symkey);
//     printf(" <-- ");
//     m_rhs->print();
//     printf(")");
// }

void NodeAssign::print()
{
    printf("assign(");
    printf(m_symkey);
    printf(" <-- ");
    m_rhs->print();
    printf(")");
}

void NodeVariable::print()
{
    printf("%s", m_symkey);
}



NodeString::NodeString( Token *t )
:   Node(Type::String),
    m_value(t->lexeme)
{

}


void NodeString::print()
{
    printf("\"%s\"", m_value);
}


NodeNumber::NodeNumber( Token *t )
:   Node(Type::Number), as_i64(atoi(t->lexeme))
{

}

void NodeNumber::print()
{
    printf("%ld", as_i64);
}







void NodeGrouping::compile( CompileCtx &ctx )
{
    for (Node *expr: m_nodes)
    {
        expr->compile(ctx);
    }
}

void NodeProgData::compile( CompileCtx &ctx )
{
    ctx.m_dataBase = m_base;
    ctx.m_dataEend = m_base + m_size;
    ctx.m_dataTop  = 0;
}


void NodeProgRoot::compile( CompileCtx &ctx )
{
    m_data->compile(ctx);
    NodeGrouping::compile(ctx);
}




void NodeBinary::compile( CompileCtx &ctx )
{
    // printf("[NodeBinary::compile] (%s, \"%s\", %s)\n",
    //     TypeToStr(m_lhs->m_type), TypeToStr(m_type), TypeToStr(m_rhs->m_type)
    // );
    m_lhs->compile(ctx);
    m_rhs->compile(ctx);

    ctx.writeText("pop rbx;\n");
    ctx.writeText("pop rax;\n");

    switch (m_type)
    {
        default: break;
        case Type::Plus:  ctx.writeText("add rax, rbx;\n"); break;
        case Type::Minus: ctx.writeText("sub rax, rbx;\n"); break;
        case Type::Star:  ctx.writeText("mul rax, rbx;\n"); break;
        case Type::Slash: ctx.writeText("div rax, rbx;\n"); break;
    }

    ctx.writeText("push rax;\n");
};



void NodeUnary::compile( CompileCtx &ctx )
{
    printf("[NodeUnary::compile] type=\"%s\"\n", TypeToStr(m_type));
    m_rhs->compile(ctx);
    ctx.writeText("mov rax, 1;\n");
    ctx.writeText("pop rbx;\n");

    switch (m_type)
    {
        default:  break;
        case Type::Bang:  ctx.writeText("error;\n"); break;
        case Type::Minus: ctx.writeText("sub rax, rbx;\npush rax;\n"); break;
    }
}




// void NodeKeyword::compile( CompileCtx &ctx )
// {
//     switch (m_type)
//     {
//         default: assert(0); break;
//         case Type::KwdLet: break;
//     }
// }



void NodeVariable::compile( CompileCtx &ctx )
{
    uintptr_t offset = (uintptr_t)sym_find(ctx.symtab, m_symkey);

    if (offset == 0x00)
    {
        offset = ctx.resb(64, 8);
        offset = (uintptr_t)sym_add(ctx.symtab, m_symkey, (void*)offset);
        ctx.writeText("resb [%lu], %lu; // [%s]\n", offset, 64, m_symkey);
    }

    ctx.writeText("push [%lu];\n", offset);
}



void NodeNumber::compile( CompileCtx &ctx )
{
    ctx.writeText("push %ld;\n", as_i64);
}

