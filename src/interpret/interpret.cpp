#include <cish/type.hpp>
#include "../parser/parser.hpp"
#include "../parser/astnode.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cish;


void AstExec::visitGroup( AstGroup *N )
{
    for (auto *child: *N)
    {
        visit(child);
    }

    printf("rax: %ld\n", m_rax);
}



void AstExec::visitCond( AstCond *N )
{
    visit(N->m_expr);
    printf(".cond_start:\n");
    printf("    jeq .cond_end\n");
    visit(N->m_body);
    printf("    jmp .cond_start\n");
    printf(".cond_end:\n");
}



void AstExec::visitBinary( AstBinary *N )
{
    visit(N->m_lhs);
    printf("    push rax\n");   *(m_top++) = m_rax;

    visit(N->m_rhs);
    printf("    mov rbx, rax\n");   m_rbx = m_rax;
    printf("    pop rax\n");        m_rax = *(--m_top);
    // printf("    add rax, rbx\n");   rax += rbx;

    // int64_t rbx = *(--m_top);
    // int64_t rax = *(--m_top);

    switch (N->m_tok->type)
    {
        default: break;
        case Type::Plus:   printf("    add rax, rbx\n"); m_rax += m_rbx; break;
        case Type::Minus:  printf("    sub rax, rbx\n"); m_rax -= m_rbx; break;
        case Type::Star:   printf("    mul rax, rbx\n"); m_rax *= m_rbx; break;
        case Type::Slash:  printf("    div rax, rbx\n"); m_rax /= m_rbx; break;
        case Type::Ampsnd: printf("    and rax, rbx\n"); m_rax &= m_rbx; break;
    }

    printf("[rax]: %ld\n", m_rax);
    // printf("    mov rax, rbx\n"); rax = rbx;
}



void AstExec::visitPrefix( AstPrefix *N )
{
    visit(N->m_rhs);
    // printf("    pop rbx\n");
    // int64_t rbx = *(--m_top);

    switch (N->m_tok->type)
    {
        default: break;
        case Type::PlusPlus:   printf("    add rax, 1\n"); m_rax += 1; break;
        case Type::MinusMinus: printf("    sub rax, 1\n"); m_rax -= 1; break;
    }
}



void AstExec::visitPostfix( AstPostfix *N )
{
    visit(N->m_lhs);

    // int64_t rax = *(--m_top);
    // *(m_top++) = rax;

    switch (N->m_tok->type)
    {
        default: break;
        case Type::PlusPlus:   printf("    add rax, 1\n"); m_rax += 1; break;
        case Type::MinusMinus: printf("    sub rax, 1\n"); m_rax -= 1; break;
    }
}



void AstExec::visitDecl( AstDecl *N )
{
    // printf("(decl ");
    // visit(N->m_target);
    // printf(")");
}


void AstExec::visitAssign( AstAssign *N )
{
    visit(N->m_expr);
    printf("    push rax\n");       *(m_top++) = m_rax;

    visit(N->m_dst);
    printf("    pop rbx\n");        m_rbx = *(--m_top);

    *((int64_t*)m_rax) = m_rbx;
    printf("[Assign] (*ptr)=%ld\n", *((int64_t*)m_rax));
    // printf("(decl ");
    // visit(N->m_target);
    // printf(")");
}



#include <bit>

void AstExec::visitVariable( AstVariable *N )
{
    // printf("    mov rax, [.varname + 0x00]\n");
    // m_rax = std::bit_cast<int64_t, int64_t*>(&(N->m_data));
}

void AstExec::visitDeclType( AstDeclType *N )
{

}

void AstExec::visitNumber( AstNumber *N )
{

}

void AstExec::visitReturn( AstReturn *N )
{
    visit(N->m_expr);
    printf("return %ld\n", m_rax);
    // printf("mov rax")
    // printf("    mov rax, [.varname + 0x00]\n");
    // m_rax = std::bit_cast<int64_t, int64_t*>(&(N->m_data));
}

























void AstPrint::visitGroup( AstGroup *N )
{
    for (auto *child: *N)
    {
        visit(child);
        if (N->m_woop)
            printf("\n");
    }
    printf("\n");
}

void AstPrint::visitCond( AstCond *N )
{
    printf("%s (", TypeToStr(N->m_tok->type));
    visit(N->m_expr);
    printf(") { ");
    visit(N->m_body);
    printf("}\n");
}

void AstPrint::visitBinary( AstBinary *N )
{
    printf("(");
    visit(N->m_lhs);
    printf(" %s ", TypeToStr(N->m_tok->type));
    visit(N->m_rhs);
    printf(")");
}

void AstPrint::visitPrefix( AstPrefix *N )
{
    printf("(%s ", TypeToStr(N->m_tok->type));
    visit(N->m_rhs);
    printf(")");
}

void AstPrint::visitPostfix( AstPostfix *N )
{
    printf("(");
    visit(N->m_lhs);
    printf(" %s)", TypeToStr(N->m_tok->type));
}

void AstPrint::visitDecl( AstDecl *N )
{
    printf("decl ");
    // visit(N->m_target);
}

void AstPrint::visitAssign( AstAssign *N )
{
    visit(N->m_dst);
    printf(" = ");
    visit(N->m_expr);
}


void AstPrint::visitVariable( AstVariable *N )
{
    printf("%s", N->m_symkey);
}


void AstPrint::visitDeclType( AstDeclType *N )
{
    printf("%s", TypeToStr(N->m_tok->type));
}


void AstPrint::visitNumber( AstNumber *N )
{
    printf("%ld", N->m_data);
}


void AstPrint::visitFunctionCall( AstFunctionCall *N )
{
    printf("call %s(", N->m_name);
    if (N->m_args)
        visit(N->m_args);
    printf(")");
}


void AstPrint::visitReturn( AstReturn *N )
{
    printf("return ");
    visit(N->m_expr);

    // printf("    mov rax, [.varname + 0x00]\n");
    // m_rax = std::bit_cast<int64_t, int64_t*>(&(N->m_data));
}
