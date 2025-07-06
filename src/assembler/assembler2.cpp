#include "assembler.hpp"
#include "symtab.hpp"
#include <stdio.h>
#include <string.h>
#include "../vm/vm.hpp"
#include "../vm/bytecode.hpp"
using namespace cish;



void visit_List( AstList& );
void visit_Binary( AstBinary& );
void visit_Prefix( AstPrefix& );
void visit_Postfix( AstPostfix& );
void visit_Scope( AstScope& );
void visit_Assign( AstAssign& );
void visit_Cond( AstCond& );
void visit_Return( AstReturn& );
void visit_Type( AstType& );
void visit_Decl( AstDecl& );
void visit_Func( AstFunc& );
void visit_Call( AstCall& );
void visit_Var( AstVar& );
void visit_String( AstString& );
void visit_Number( AstNumber& );


/*
    https://www.reddit.com/r/Compilers/comments/18wjm7g/how_do_variables_work_under_the_hood/

    Well, typically on x64, rbp is pushed, rsp is copied to rbp,
    and rsp is decremented to 'allocate' space for the stack frame
    which contains space for the locals, any extra temporaries the
    compilers will need, and take care of things like keeping rsp
    16-byte aligned.

    At the end, rsp is restored, and rbp is popped. All this depends
    heavily on ABI, language, compiler, and compiler options. 


    push rbp
    mov rbp, rsp
    rsp += sizeof(local variables)
    {
        // local scope stuff
    }
    mov rsp, rbp // restore rsp

*/



static uint32_t *op_top;
static ProgramScope *pScope;

template <typename T>
static T *emit( uint8_t opcode, const T &data )
{
    T *ptr = new (op_top) T(data);
    ptr->opcode = opcode;
    op_top = ptr->next;
    return ptr;
}

static void emit( uint8_t opcode )
{
    emit(opcode, VmOp());
    // VmOp *ptr = new (op_top) VmOp();
    // ptr->opcode = opcode;
    // op_top++;
}

// static void emit( const VmOp &op )
// {
//     // op_top->opcode = opcode;
//     *(op_top++) = op;
// }

// static void emit( VmOp_ opcode, uint64_t data )
// {
//     newVmOp(opcode, VmOpData())
//     VmOpPush *opp = (VmOpPush*)op_top;
//     opp->op.opcode = opcode;
//     opp->data = data;
//     op_top = opp->next;
// }


static void visit( AstNode *N )
{
    switch (N->type)
    {
        default: return;
        case Ast_List:      visit_List(N->as_List);         break;
        case Ast_Binary:    visit_Binary(N->as_Binary);     break;
        case Ast_Prefix:    visit_Prefix(N->as_Prefix);     break;
        case Ast_Postfix:   visit_Postfix(N->as_Postfix);   break;
        case Ast_Scope:     visit_Scope(N->as_Scope);       break;
        case Ast_Assign:    visit_Assign(N->as_Assign);     break;
        case Ast_Cond:      visit_Cond(N->as_Cond);         break;
        case Ast_Return:    visit_Return(N->as_Return);     break;
        case Ast_Type:      visit_Type(N->as_Type);         break;
        case Ast_Decl:      visit_Decl(N->as_Decl);         break;
        case Ast_Func:      visit_Func(N->as_Func);         break;
        case Ast_Call:      visit_Call(N->as_Call);         break;
        case Ast_Var:       visit_Var(N->as_Var);           break;
        case Ast_String:    visit_String(N->as_String);     break;
        case Ast_Number:    visit_Number(N->as_Number);     break;
    }

}


uint32_t *cish::assemble( AstNode *ast, uint32_t *buf, size_t bufsz )
{
    op_top = buf;
    pScope = new ProgramScope(new symtab_t[512]);

    emit(VmOp_pushd, VmOpData(123454321));
    visit(ast);
    emit(VmOp_exit);

    return nullptr;
}



void visit_List( AstList &N )
{
    for (auto *child: N)
    {
        visit(child);
    }
}



void visit_Binary( AstBinary &N )
{
    visit(N.m_lhs);
    // emit(VmOp_popr, VmOpReg(Reg_rax));

    visit(N.m_rhs);
    // emit(VmOp_popr, VmOpReg(Reg_rbx));
    // auto oprr = VmOpRegReg(Reg_rax, Reg_rbx);

    switch (N.m_tok->type)
    {
        default: break;
        // case Type::Plus:   emit(VmOp_addrr, oprr); break;
        // case Type::Minus:  emit(VmOp_subrr, oprr); break;
        // case Type::Star:   emit(VmOp_mulrr, oprr); break;
        // case Type::Slash:  emit(VmOp_divrr, oprr); break;
        // case Type::Ampsnd: emit(VmOp_andrr, oprr); break;
        // case Type::Bar:    emit(VmOp_orrr,  oprr); break;
        // case Type::Hat:    emit(VmOp_xorrr, oprr); break;
        case Type::Plus:   emit(VmOp_adds); break;
        case Type::Minus:  emit(VmOp_subs); break;
        case Type::Star:   emit(VmOp_muls); break;
        case Type::Slash:  emit(VmOp_divs); break;
        case Type::Ampsnd: emit(VmOp_ands); break;
        case Type::Bar:    emit(VmOp_ors);  break;
        case Type::Hat:    emit(VmOp_xors); break;
    }

    // emit(VmOp_pushr, VmOpReg(Reg_rax));
}



void visit_Prefix( AstPrefix &N )
{

}



void visit_Postfix( AstPostfix &N )
{

}



void visit_Scope( AstScope &N )
{
    pScope->push();
    emit(VmOp_pushr, VmOpReg(Reg_rbp));             // push rbp
    emit(VmOp_movrr, VmOpRegReg(Reg_rbp, Reg_rsp)); // mov  rbp, rsp

    visit(N.m_body);

    emit(VmOp_movrr, VmOpRegReg(Reg_rsp, Reg_rbp)); // mov  rsp, rbp
    pScope->pop();
}



void visit_Assign( AstAssign &N )
{
    visit(N.m_expr);
}



void visit_Cond( AstCond &N )
{
    visit(N.m_cond);
}



void visit_Return( AstReturn &N )
{
    visit(N.m_expr);
    emit(VmOp_ret);
    // printf("ret\n");
}



void visit_Type( AstType &N )
{

}



void visit_Decl( AstDecl &N )
{
    AstType &dtype = N.m_dtype->as_Type;
    pScope->insert(N.m_symkey, dtype.m_size);

    emit(VmOp_addrd, VmOpRegData(Reg_rsp, dtype.m_size));
    printf("addrd %u, %lu\n", Reg_rsp, dtype.m_size);

}



void visit_Func( AstFunc &N )
{

}



void visit_Call( AstCall &N )
{

}



void visit_Var( AstVar &N )
{
    // offset relative to rbp
    const auto &[key, offset, size] = pScope->find(N.m_symkey);

    if (offset == -1)
    {
        fprintf(stderr, "[visit_Var] Ruh roh\n");
        exit(1);
    }

    // mov rax, [rsp + offset]
    emit(VmOp_movrrd, VmOpRegRegData(Reg_rax, Reg_rsp, offset));

    // push rax
    emit(VmOp_pushr, VmOpReg(Reg_rax));


    // printf("movrrd %u, [%u + %d]\n", Reg_rax, Reg_rsp, offset);
    // printf("pushr %u\n", Reg_rax);
}



void visit_String( AstString &N )
{

}



void visit_Number( AstNumber &N )
{
    int64_t value = atol(N.m_str);
    emit(VmOp_pushd, VmOpData(value));

    // printf("pushd %ld\n", value);
}

