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

static void pass1( CompileCtx&, AstNode* );


void compile_pass1( CompileCtx &ctx, AstNode *ast )
{
    std::cout << "---------------- PASS 1 ----------------\n";
    ctx.ripReset();
    pass1(ctx, ast);
    std::cout << "----------------------------------------\n\n";
}


static void pass1_List( CompileCtx &ctx, AstList &N )
{
    for (auto *child: N)
    {
        pass1(ctx, child);
    }
}



static void pass1_Binary( CompileCtx &ctx, AstBinary &N )
{
    pass1(ctx, N.m_lhs);
    pass1(ctx, N.m_rhs);
}



static void pass1_Prefix( CompileCtx &ctx, AstPrefix &N )
{

}



static void pass1_Postfix( CompileCtx &ctx, AstPostfix &N )
{

}



static void pass1_Scope( CompileCtx &ctx, AstScope &N )
{
    pass1(ctx, N.m_body);
}



static void pass1_Assign( CompileCtx &ctx, AstAssign &N )
{
    pass1(ctx, N.m_expr);
}



static void pass1_Cond( CompileCtx &ctx, AstCond &N )
{
    pass1(ctx, N.m_cond);
}


static void pass1_Call( CompileCtx &ctx, AstCall &N )
{
    auto *sym = ctx.findSymbol(N.m_callee);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Func));
}


static void pass1_Return( CompileCtx &ctx, AstReturn &N )
{
    pass1(ctx, N.m_expr);
}


static void pass1_Type( CompileCtx &ctx, AstType &N )
{

}


static void pass1_VarDecl( CompileCtx &ctx, AstVarDecl &N )
{
    // Symbol *sym = nullptr;

    // sym = ctx.findSymbol(N.m_typename);
    // assert((sym != nullptr));
    // assert((sym->tag == Sym_Type));

    // SymType &tsym = sym->as_Type;
    // size_t  addr = ctx.frameAlloc(tsym.size, tsym.align);
    // sym = ctx.createSymbol(N.m_name, SymVar(N.m_typename, addr));
    // assert((sym != nullptr));
    // assert((sym->tag == Sym_Var));

    // SymVar &vsym = sym->as_Var;

    // printf("%s %s, [vbp + %lu]\n", vsym.type_name, sym->key, vsym.addr);
}


static void pass1_FunDecl( CompileCtx &ctx, AstFunDecl &N )
{
    // Symbol *sym = nullptr;

    // sym = ctx.createSymbol(N.m_name, SymFunc(N.m_ret_typename, 0));
    // assert((sym != nullptr));
    // assert((sym->tag == Sym_Func));
    // SymFunc &fsym = sym->as_Func;

    // ctx.pushScope();
    // pass1(ctx, N.m_args);
    // pass1(ctx, N.m_body);
    // fsym.argc = N.m_args->as_List.size();
    // fsym.resb = ctx.topScope().allocSize();
    // ctx.popScope();

    // printf(
    //     "%s %s(%lu), [%lu], resb %lu\n",
    //     fsym.return_type, sym->key, fsym.argc, fsym.addr, fsym.resb
    // );
}




static void pass1_Var( CompileCtx &ctx, AstVar &N )
{
    // auto [depth, sym] = ctx.findObject(N.m_symkey);
    // assert((sym != nullptr));

    // ctx.emit(VmOp_vload32, Reg_rax, sym->addr);
    // ctx.emit(VmOp_pushrg, Reg_rax);
}



static void pass1_String( CompileCtx &ctx, AstString &N )
{

}



static void pass1_Number( CompileCtx &ctx, AstNumber &N )
{
    int64_t value = atol(N.m_str);
    // ctx.emit(VmOp_push32, value);
}







static void pass1( CompileCtx &ctx, AstNode *N )
{
    switch (N->type)
    {
        default: return;
        case Ast_List:      pass1_List(ctx, N->as_List);         break;
        case Ast_Binary:    pass1_Binary(ctx, N->as_Binary);     break;
        case Ast_Prefix:    pass1_Prefix(ctx, N->as_Prefix);     break;
        case Ast_Postfix:   pass1_Postfix(ctx, N->as_Postfix);   break;
        case Ast_Scope:     pass1_Scope(ctx, N->as_Scope);       break;
        case Ast_Assign:    pass1_Assign(ctx, N->as_Assign);     break;
        case Ast_Cond:      pass1_Cond(ctx, N->as_Cond);         break;
        case Ast_Call:      pass1_Call(ctx, N->as_Call);         break;
        case Ast_Return:    pass1_Return(ctx, N->as_Return);     break;
        case Ast_Type:      pass1_Type(ctx, N->as_Type);         break;
        case Ast_VarDecl:   pass1_VarDecl(ctx, N->as_VarDecl);   break;
        case Ast_FunDecl:   pass1_FunDecl(ctx, N->as_FunDecl);   break;
        case Ast_Var:       pass1_Var(ctx, N->as_Var);           break;
        case Ast_String:    pass1_String(ctx, N->as_String);     break;
        case Ast_Number:    pass1_Number(ctx, N->as_Number);     break;
    }

}

