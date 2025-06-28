#include <cish/node.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cish;


void NodeGrouping::print()
{
    // printf("Grouping[");

    for (auto *N: m_nodes)
    {
        N->print();

        if (N->m_type != Type::Grouping)
            printf(";\n");
    }
    // printf("]; ");
}

void NodeBinary::print()
{
    printf("(");
    m_lhs->print();
    (m_optype) ? printf(" %s ", TypeToStr(m_optype)) : printf(", ");
    m_rhs->print();
    printf(")");
}





// void NodeAssign::print()
// {
//     // printf("assign(");
//     // m_lhs->print();
//     // printf(", ");
//     // m_rhs->print();
//     // printf(")");
//     m_lhs->print();
//     printf(" = ");
//     m_rhs->print();
// }



void NodeUnary::print()
{
    printf("(%s", TypeToStr(m_optype));
    m_rhs->print();
    printf(")");
}


void NodeLeaf::print()
{
    printf("%s", m_str);
}



void NodeDecl::print()
{
    printf("declare(%s: %s)", m_target, m_dtype);
}


NodeNumber::NodeNumber( Token *t )
:   NodeLeaf(Type::Number, t)
{
    // switch (m_optype)
    // {
    //     default: break;
    //     case Type::i32: as_i32 = atoi(t->lexeme);  break;
    //     case Type::i64: as_i64 = atoll(t->lexeme); break;
    //     case Type::u32: as_u32 = atol(t->lexeme);  break;
    //     case Type::u64: as_u64 = atoll(t->lexeme); break;
    // }

    as_i64 = atoi(t->lexeme);
}

void NodeNumber::print()
{
    // switch (m_optype)
    // {
    //     default: break;
    //     case Type::i32: printf("ld", as_i32); break;
    //     case Type::i64: printf("ld", as_i64); break;
    //     case Type::u32: printf("ud", as_u32); break;
    //     case Type::u64: printf("ud", as_u64); break;
    // }
    printf("%ld", as_i64);
}













#define PUSH(x_) (*(rsp++) = (x_))
#define POP() (*(--rsp))


void NodeGrouping::compile( Node*, int64_t *&rsp )
{
    // int64_t *tmp = rsp;

    printf("[NodeGrouping::compile]\n");
    for (Node *expr: m_nodes)
    {
        expr->compile(this, rsp);
    }

    // printf("top: %ld\n", rsp-tmp, *(rsp));
};



void NodeBinary::compile( Node*, int64_t *&rsp )
{
    printf("[NodeBinary::compile]\n");
    printf("lhs->type: %s\n", TypeToStr(m_lhs->m_type));
    m_lhs->compile(this, rsp);
    printf("rhs->type: %s\n", TypeToStr(m_rhs->m_type));
    m_rhs->compile(this, rsp);
    // *(rsp++) = 5;
    // *(rsp++) = 3;

    printf("pop rbx;\n");
    int64_t rbx = *(--rsp);
    printf("pop rax;\n");
    int64_t rax = *(--rsp);

    switch (m_optype)
    {
        default: break;
        case Type::Plus:  printf("add rax, rbx;\n"); rax += rbx; break;
        case Type::Minus: printf("sub rax, rbx;\n"); rax -= rbx; break;
        case Type::Star:  printf("mul rax, rbx;\n"); rax *= rbx; break;
        case Type::Slash: printf("div rax, rbx;\n"); rax /= rbx; break;
    }

    *(rsp++) = rax;
    printf("push rax;\n");

};



void NodeDecl::compile( Node *P, int64_t *&rsp )
{
    printf("[NodeDecl::compile]\n");
    auto *idtype = DataPrimitive::as_type<int64_t>("i64");

    printf("align %ld;\n", idtype->m_align);
    printf("resb %s, %ld;\n\n", m_target, idtype->m_size);
}


#include <cish/symtab.hpp>

void NodeIdentifier::compile( Node*, int64_t *&rsp )
{
    printf("[NodeIdentifier::compile]\n");
    // void *symtab;

    // if (!cish::sym_find(symtab, m_str))
    // {
    //     // cish::sym_add(symtab, m_str, rsp);
    //     // rsp += align
    // }

    // int64_t *ptr = (int64_t*)cish::sym_find(symtab, m_str);
    // PUSH(*ptr);

    // PUSH(12345);
}

void NodeNumber::compile( Node *P, int64_t *&rsp )
{
    printf("[NodeNumber::compile]\n");
    // if (P->m_type == Type::Binary)
    // {
    //     const char *reg = "";
    //     if (this == ((NodeBinary*)P)->m_lhs) reg="rax";
    //     if (this == ((NodeBinary*)P)->m_rhs) reg="rbx";
    //     printf("mov %s, %ld;\n", as_i64);
    // }

    // else
    // {
        printf("push %ld;\n", as_i64);
        *(rsp++) = as_i64;
    // }

    // switch (m_optype)
    // {
    //     default: break;
    //     case Type::i32: printf("push %d; ",  as_i32); break;
    //     case Type::i64: printf("push %ld; ", as_i64); break;
    //     case Type::u32: printf("push %u; ",  as_u32); break;
    //     case Type::u64: printf("push %lu; ", as_u64); break;
    // }
}

