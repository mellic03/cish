#include "../vm/emit.hpp"
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

    // Emit_movImm(ctx, Reg_vbp, 32);
    // Emit_movReg(ctx, Reg_vsp, Reg_vbp);
    // Emit_addvsp(ctx, 1); // [vbp + 0]
    // Emit_addvsp(ctx, 1); // [vbp + 1]

    // Emit_pushImm(ctx, 2);
    // Emit_vstor(ctx, 0);

    // Emit_pushImm(ctx, 10);
    // Emit_vstor(ctx, 1);

    // Emit_vload(ctx, 0);
    // Emit_vload(ctx, 1);
    // Emit_mul(ctx);
    // Emit_ret(ctx);
    // ctx.emit(VmOp_exit);
    // return;

    VmOp *jmp = ctx.emit(VmOp_CompileIR);
    pass2(ctx, ast);
    ctx.emit(VmOp_exit);

    auto &tab = ctx.getGlobal();
    auto *sym = tab.find("main");
    assert((sym != nullptr));
    assert((sym->tag == Sym_Func));

    *jmp = Op_jmp(sym->as_Func.addr);

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
    pass2(ctx, N.m_rhs);
    Emit_swap(ctx);

    switch (N.m_tok->type)
    {
        default: return;
        case Type::Plus:         Emit_add(ctx); break;
        case Type::Minus:        Emit_sub(ctx); break;
        case Type::Star:         Emit_mul(ctx); break;
        case Type::Slash:        Emit_div(ctx); break;
        case Type::Ampsnd:       Emit_and(ctx); break;
        case Type::Bar:          Emit_or(ctx);  break;
        case Type::Hat:          Emit_xor(ctx); break;
        case Type::EqualEqual:   Emit_equ(ctx); break;
        case Type::Less:         Emit_les(ctx); break;
        case Type::LessEqual:    Emit_leq(ctx); break;
        case Type::Greater:      Emit_gtr(ctx); break;
        case Type::GreaterEqual: Emit_geq(ctx); break;
    }
}



static void pass2_Prefix( CompileCtx &ctx, AstPrefix &N )
{

}



static void pass2_Postfix( CompileCtx &ctx, AstPostfix &N )
{

}


static void pass2_Assign( CompileCtx &ctx, AstAssign &N )
{
    auto &tab = ctx.getLocal();
    auto *sym = tab.find(N.m_var);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Var));
    SymVar &vsym = sym->as_Var;

    pass2(ctx, N.m_expr);

    // pop [rbp + vsym.addr]
    Emit_vstor(ctx, vsym.rbpoff);
}



static void pass2_Cond( CompileCtx &ctx, AstCond &N )
{
    if (N.m_kwd->type == Type::KwdIf)
    {
        pass2(ctx, N.m_cond);

        // pop rcmp0
        // mov rcmp1, 0
        Emit_popReg(ctx, Reg_rcmp0);
        Emit_movImm(ctx, Reg_rcmp1, 0);

        VmOp *jmpToElse, *jmpToEnd;
        size_t atCond, atIf, atElse, atEnd;

        // if (rcmp0 == rcmp1) jmp atElse
        // if (rcmp0 == 0)     jmp atElse
        atCond = ctx.rip();
            jmpToElse = ctx.emit(VmOp_CompileIR);

        atIf = ctx.rip();
            pass2(ctx, N.m_if);
            jmpToEnd = ctx.emit(VmOp_CompileIR);

        atElse = ctx.rip();
            pass2(ctx, N.m_else);

        atEnd = ctx.rip();
            // Nothing here

        *jmpToElse = Op_jeq(atElse);
        *jmpToEnd  = Op_jmp(atEnd);
        // *jmpToElse = VmOpRegRegImm(VmOp_jne, Reg_r8, Reg_r9, atElse);
        // *jmpToEnd  = VmOpImm(VmOp_jmp, atEnd);
    }

    // else if (N.m_kwd->type == Type::KwdWhile)
    // {
    //     VmOp *jmpToCond, *jmpToEnd;
    //     size_t atCond, atBody, atEnd;

    //     atCond = ctx.rip();
    //         pass2(ctx, N.m_cond);
    //         ctx.emit(VmOpReg(VmOp_pop32r, Reg_r8));
    //         ctx.emit(VmOpRegImm(VmOp_mov32, Reg_r9, 0));
    //         jmpToEnd = ctx.emit(VmOp_CompileIR);

    //     atBody = ctx.rip();
    //         pass2(ctx, N.m_if);
    //         jmpToCond = ctx.emit(VmOp_CompileIR);

    //     atEnd = ctx.rip();
    //         // Nothing here

    //     *jmpToEnd  = VmOpRegRegImm(VmOp_jne, Reg_r8, Reg_r9, atEnd);
    //     *jmpToCond = VmOpImm(VmOp_jmp, atCond);
    // }

}


static void pass2_Call( CompileCtx &ctx, AstCall &N )
{
    Symtab &tab = ctx.getLocal();
    Symbol *sym = tab.find(N.m_callee);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Func));
    SymFunc &fsym = sym->as_Func;

    Emit_call(ctx, fsym.addr);
    Emit_pushReg(ctx, Reg_rax);
    // ctx.emit(VmOpImm(VmOp_call, fsym.addr));
}


static void pass2_Return( CompileCtx &ctx, AstReturn &N )
{
    pass2(ctx, N.m_expr);
    Emit_popReg(ctx, Reg_rax);
    Emit_ret(ctx);
}


static void pass2_Type( CompileCtx &ctx, AstType &N )
{

}



static void pass2_VarDecl( CompileCtx &ctx, AstVarDecl &N )
{
    Symtab &tab = ctx.getLocal();
    Symbol *sym = nullptr;

    sym = tab.find(N.m_typename);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Type));
    SymType &tsym = sym->as_Type;
    size_t addr = tab.frameAlloc(tsym.size, 1);

    sym = tab.insert(N.m_name, SymVar(N.m_typename, addr));
    assert((sym != nullptr));
    assert((sym->tag == Sym_Var));
    SymVar &vsym = sym->as_Var;

    // add rsp, tsym.size
    Emit_addImm(ctx, Reg_vsp, tsym.size);

    printf("let %s %s &[vbp + %lu]\n", N.m_typename, N.m_name, addr);

}


static void pass2_FunDecl( CompileCtx &ctx, AstFunDecl &N )
{
    Symtab &tab = ctx.getLocal();
    Symbol *sym = tab.insert(N.m_name, SymFunc(N.m_ret_typename, ctx.rip()));
    assert((sym != nullptr));
    assert((sym->tag == Sym_Func));

    SymFunc &fsym = sym->as_Func;
    printf("[%lu] %s\n", fsym.addr, sym->key);

    ctx.pushLocal();
    pass2(ctx, N.m_args);
    pass2(ctx, N.m_body);
    ctx.popLocal();

    fsym.argc = N.m_args->as_List.size();
}




static void pass2_Var( CompileCtx &ctx, AstVar &N )
{
    Symtab &tab = ctx.getLocal();
    Symbol *sym = tab.find(N.m_symkey);
    assert((sym != nullptr));
    assert((sym->tag == Sym_Var));

    SymVar &vsym = sym->as_Var;

    // push [rbp + vsym.rbpoff]
    Emit_vload(ctx, vsym.rbpoff);
}



static void pass2_String( CompileCtx &ctx, AstString &N )
{

}



static void pass2_Number( CompileCtx &ctx, AstNumber &N )
{
    int64_t value = atol(N.m_str);
    Emit_pushImm(ctx, value);
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

