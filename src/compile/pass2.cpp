#include "../vm/bytecode.hpp"
#include "../vm/ctx.hpp"
#include "../vm/vm.hpp"
#include <cish/compile.hpp>
#include <cish/compile-ctx.hpp>
#include <cish/node.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>

using namespace cish;

static void pass2( CompileCtx&, AstNode* );



void compile_pass2( CompileCtx &ctx, AstNode *ast )
{
    std::cout << "---------------- PASS 2 ----------------\n";
    ctx.ripReset();

    VmOp *jmp = ctx.emit(VmOp_jmp, 0);
    pass2(ctx, ast);
    ctx.emit(VmOp_exit);

    auto *sym = ctx.findSymbol("main");
    assert((sym != nullptr));
    assert((sym->tag == Sym_Func));
    jmp->d32 = sym->as_Func.addr;

    std::cout << "----------------------------------------\n\n";
}

// uint32_t *cish::compile( AstNode *ast, uint32_t *buf, size_t bufsz )
// {
//     ctx = cish::CompileCtx(buf, bufsz);
//     VmOp *jp = ctx.emit(VmOp_jmp, 0);

//     pass2(ast);
//     ctx.emit(VmOp_exit);

//     funcsym_t *sym = ctx.findFunction("main");
//     assert((sym != nullptr));
//     jp->d32 = sym->addr;

//     return nullptr;
// }




static void pass2_List( CompileCtx &ctx, AstList &N )
{
    for (auto *child: N)
    {
        pass2(ctx, child);
    }
}



static void pass2_Binary( CompileCtx &ctx, AstBinary &N )
{
    pass2(ctx, N.m_lhs);
    ctx.emit(VmOpReg(VmOp_push32r, Reg_rax));

    pass2(ctx, N.m_rhs);
    ctx.emit(VmOpReg(VmOp_push32r, Reg_rax));
    ctx.emit(VmOp_swap32);

    switch (N.m_tok->type)
    {
        default: break;
        case Type::Plus:         ctx.emit(VmOp_add);  break;
        case Type::Minus:        ctx.emit(VmOp_sub);  break;
        case Type::Star:         ctx.emit(VmOp_mul);  break;
        case Type::Slash:        ctx.emit(VmOp_div);  break;
        case Type::Ampsnd:       ctx.emit(VmOp_and);  break;
        case Type::Bar:          ctx.emit(VmOp_or);   break;
        case Type::Hat:          ctx.emit(VmOp_xor);  break;
        case Type::EqualEqual:   ctx.emit(VmOp_equ);  break;
        case Type::Less:         ctx.emit(VmOp_les);  break;
        case Type::LessEqual:    ctx.emit(VmOp_leq);  break;
        case Type::Greater:      ctx.emit(VmOp_gtr);  break;
        case Type::GreaterEqual: ctx.emit(VmOp_geq);  break;
    }
}



static void pass2_Prefix( CompileCtx &ctx, AstPrefix &N )
{

}



static void pass2_Postfix( CompileCtx &ctx, AstPostfix &N )
{

}



static void pass2_Scope( CompileCtx &ctx, AstScope &N )
{
    // pScope->push();
    // ctx.emit(VmOp_pushr, VmOpReg(Reg_rbp));             // push rbp
    // ctx.emit(VmOp_movrr, VmOpRegReg(Reg_rbp, Reg_rsp)); // mov  rbp, rsp

    pass2(ctx, N.m_body);

    // ctx.emit(VmOp_movrr, VmOpRegReg(Reg_rsp, Reg_rbp)); // mov  rsp, rbp
    // pScope->pop();
}



static void pass2_Assign( CompileCtx &ctx, AstAssign &N )
{
    pass2(ctx, N.m_expr);

    Symbol *sym = ctx.findSymbol(N.m_var);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Var));
    SymVar &vsym = sym->as_Var;

    ctx.emit(VmOpReg(VmOp_push32r, Reg_rax));
    ctx.emit(VmOpData(VmOp_vstor, vsym.addr));
}



static void pass2_Cond( CompileCtx &ctx, AstCond &N )
{
    pass2(ctx, N.m_cond);            // push expr
    ctx.emit(VmOp_push32d, 0);   // push 0/false

    if (N.m_kwd->type == Type::KwdIf)
    {
        VmOp *jeq = ctx.emit(VmOp_jeq, 0);  // jmp d32 if [rsp-1] == [rsp-2]

        pass2(ctx, N.m_body);
        ctx.emit(VmOp_nop);
        size_t cond_end = ctx.rip();

        jeq->d32 = cond_end;
    }

    // else if (N.m_kwd->type == Type::KwdWhile)
    // {
    //     size_t loop_beg = op_top-op_base;
    //     auto *jeqd = ctx.emit(VmOp_jeqd, VmOpData(0));  // if expr==0: jmp to loop_end

    //     pass2(ctx, N.m_body);
    //     ctx.emit(VmOp_jeqd, VmOpData(loop_beg));        // jmp to loop_beg

    //     size_t loop_end = op_top-op_base;
    //     jeqd->data = VmOpU64(loop_end);
    // }

}


static void pass2_Call( CompileCtx &ctx, AstCall &N )
{
    Symbol *sym = ctx.findSymbol(N.m_callee);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Func));
    SymFunc &fsym = sym->as_Func;

    ctx.emit(VmOpData(VmOp_call, fsym.addr));
}


static void pass2_Return( CompileCtx &ctx, AstReturn &N )
{
    pass2(ctx, N.m_expr);
    ctx.emit(VmOp_ret);
}


static void pass2_Type( CompileCtx &ctx, AstType &N )
{

}



static void pass2_VarDecl( CompileCtx &ctx, AstVarDecl &N )
{
    Symbol *sym = nullptr;

    sym = ctx.findSymbol(N.m_typename);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Type));

    SymType &tsym = sym->as_Type;
    size_t addr = ctx.frameAlloc(tsym.size, 1);
    ctx.emit(VmOpData(VmOp_vpush32d, tsym.size));

    sym = ctx.createSymbol(N.m_name, SymVar(N.m_typename, addr));
    assert((sym != nullptr));
    assert((sym->tag == Sym_Var));
    SymVar &vsym = sym->as_Var;

    printf("%s %s, V[%lu]\n", N.m_typename, N.m_name, vsym.addr);
}


static void pass2_FunDecl( CompileCtx &ctx, AstFunDecl &N )
{
    Symbol *sym = nullptr;

    sym = ctx.createSymbol(N.m_name, SymFunc(N.m_ret_typename, ctx.rip()));
    assert((sym != nullptr));
    assert((sym->tag == Sym_Func));

    SymFunc &fsym = sym->as_Func;
    printf("[%lu] %s\n", fsym.addr, sym->key);

    ctx.pushScope();
    pass2(ctx, N.m_args);
    pass2(ctx, N.m_body);
    fsym.argc = N.m_args->as_List.size();
    ctx.popScope();

    // ctx.emit(VmOp_movrg,  Reg_vsp, Reg_vbp); // mov rsp, rbp
    // ctx.emit(VmOp_vpoprg, Reg_vbp);          // pop rbp
}




static void pass2_Var( CompileCtx &ctx, AstVar &N )
{
    Symbol *sym = ctx.findSymbol(N.m_symkey);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Var));

    SymVar &vsym = sym->as_Var;

    ctx.emit(VmOpData(VmOp_vload, vsym.addr));
    ctx.emit(VmOpReg(VmOp_pop32r, Reg_rax));
    // printf("vload V[%lu]\n", vsym.addr);
}



static void pass2_String( CompileCtx &ctx, AstString &N )
{

}



static void pass2_Number( CompileCtx &ctx, AstNumber &N )
{
    int64_t value = atol(N.m_str);
    ctx.emit(VmOpRegData(VmOp_mov32, Reg_rax, value));
}








static void pass2( CompileCtx &ctx, AstNode *N )
{
    switch (N->type)
    {
        default: return;
        case Ast_List:      pass2_List(ctx, N->as_List);         break;
        case Ast_Binary:    pass2_Binary(ctx, N->as_Binary);     break;
        case Ast_Prefix:    pass2_Prefix(ctx, N->as_Prefix);     break;
        case Ast_Postfix:   pass2_Postfix(ctx, N->as_Postfix);   break;
        case Ast_Scope:     pass2_Scope(ctx, N->as_Scope);       break;
        case Ast_Assign:    pass2_Assign(ctx, N->as_Assign);     break;
        case Ast_Cond:      pass2_Cond(ctx, N->as_Cond);         break;
        case Ast_Call:      pass2_Call(ctx, N->as_Call);         break;
        case Ast_Return:    pass2_Return(ctx, N->as_Return);     break;
        case Ast_Type:      pass2_Type(ctx, N->as_Type);         break;
        case Ast_VarDecl:   pass2_VarDecl(ctx, N->as_VarDecl);   break;
        case Ast_FunDecl:   pass2_FunDecl(ctx, N->as_FunDecl);   break;
        case Ast_Var:       pass2_Var(ctx, N->as_Var);           break;
        case Ast_String:    pass2_String(ctx, N->as_String);     break;
        case Ast_Number:    pass2_Number(ctx, N->as_Number);     break;
    }

}

