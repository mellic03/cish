#include "./pass1.hpp"

using namespace cish;


// void CompilePass_1::visit_List( CompileCtx &ctx, List &N )
// {

// }


// void CompilePass_1::visit_Atom( CompileCtx &ctx, Atom &N )
// {

// }


// void CompilePass_1::visit_Program( CompileCtx &ctx, Program &N )
// {

// }


// void CompilePass_1::visit_Stmnt( CompileCtx &ctx, Stmnt &N )
// {

// }


// void CompilePass_1::visit_StmntList( CompileCtx &ctx, StmntList &N )
// {

// }


// void CompilePass_1::visit_Decl( CompileCtx &ctx, Decl &N )
// {

// }


// void CompilePass_1::visit_Assign( CompileCtx &ctx, Assign &N )
// {

// }


// void CompilePass_1::visit_Expr( CompileCtx &ctx, Expr &N )
// {

// }


// void CompilePass_1::visit_VarDecl( CompileCtx &ctx, VarDecl &N )
// {
//     // ((ProdNode*)&N)->as_Assign.
// }


// void CompilePass_1::visit_FunDecl( CompileCtx &ctx, FunDecl &N )
// {

// }


// void CompilePass_1::visit_DeclList( CompileCtx &ctx, DeclList &N )
// {

// }


// void CompilePass_1::visit_Call( CompileCtx &ctx, Call &N )
// {

// }


// void CompilePass_1::visit_CallList( CompileCtx &ctx, CallList &N )
// {

// }


// void CompilePass_1::visit_Term( CompileCtx &ctx, Term &N )
// {

// }






// #include "../vm/bytecode.hpp"
// #include "../vm/ctx.hpp"
// #include "../vm/vm.hpp"
// #include <cish/compile.hpp>
// #include <cish/compile-ctx.hpp>
// #include <cish/node.hpp>
// #include <iostream>
// #include <stdio.h>
// #include <string.h>
// #include <assert.h>

// using namespace cish;

// static void visit( CompileCtx&, AstNode* );


// // Compute local variable stack frame offsets.
// void compile_pass1( CompileCtx &ctx, AstNode *ast )
// {
//     std::cout << "---------------- PASS 1 ----------------\n";
//     ctx.clearRegs();
//     visit(ctx, ast);
//     std::cout << "----------------------------------------\n\n";
// }


// static void visit_List( CompileCtx &ctx, AstList &N )
// {
//     for (auto *child: N)
//     {
//         visit(ctx, child);
//         if (child->m_type != Ast_List)
//             printf("\n");
//     }
// }



// static void visit_Binary( CompileCtx &ctx, AstBinary &N )
// {
//     printf("(");
//     visit(ctx, N.m_lhs);
//     printf(" %s ", N.m_tok->lexeme);
//     visit(ctx, N.m_rhs);
//     printf(")");
// }



// static void visit_Prefix( CompileCtx &ctx, AstPrefix &N )
// {

// }



// static void visit_Postfix( CompileCtx &ctx, AstPostfix &N )
// {

// }



// static void visit_Scope( CompileCtx &ctx, AstScope &N )
// {
//     visit(ctx, N.m_body);
// }



// static void visit_Assign( CompileCtx &ctx, AstAssign &N )
// {
//     printf("%s = ", N.m_var);
//     visit(ctx, N.m_expr);
// }



// static void visit_Cond( CompileCtx &ctx, AstCond &N )
// {
//     visit(ctx, N.m_cond);
// }


// static void visit_Call( CompileCtx &ctx, AstCall &N )
// {
//     printf("%s(", N.m_symkey);
//     if (N.m_expr)
//         visit(ctx, N.m_expr);
//     printf(")");

//     // SymTab &tab = ctx.getLocal();
//     // Symbol *sym = tab.find(N.m_callee);
//     // assert((sym != nullptr));
//     // assert((sym->tag == Sym_Func));
// }


// static void visit_Return( CompileCtx &ctx, AstReturn &N )
// {
//     printf("return ");
//     visit(ctx, N.m_expr);
// }


// static void visit_VarDecl( CompileCtx &ctx, AstVarDecl &N )
// {
//     SymTab *tab = ctx.localTab();

//     auto [tsym, trhs] = tab->find<SymType>(N.m_typekey);
//     auto [vsym, vrhs] = tab->insert(N.m_name, SymVar(N.m_typekey));
//     vsym->offset = ctx.resvFrame(tsym->size, tsym->align);

//     printf("%s %s &[%lu] depth=%d\n", vsym->typekey, vrhs->key, vsym->offset, tab->depth);
// }


// static void visit_FunDecl( CompileCtx &ctx, AstFunDecl &N )
// {
//     SymTab *tab = ctx.localTab();
    
//     auto [fsym, frhs] = tab->insert(N.m_name, SymFunc(N.m_ret_typename));
//     printf("func %s %s()\tdepth=%d\n", fsym->retkey, frhs->key, tab->depth);

//     ctx.pushFrame();
//     visit(ctx, N.m_args);
//     visit(ctx, N.m_body);
//     fsym->allocsz = ctx.rbp() - ctx.rsp();
//     printf("endfunc, allocsz=%lu\n", fsym->allocsz);
//     ctx.popFrame();
  
// }




// static void visit_Var( CompileCtx &ctx, AstVar &N )
// {
//     SymTab *tab = ctx.localTab();
//     // tab->insert(N.m_symkey, SymVar(N.))
//     // printf("%s", N.m_symkey);
// }



// static void visit_String( CompileCtx &ctx, AstString &N )
// {

// }



// static void visit_Number( CompileCtx &ctx, AstNumber &N )
// {
//     int64_t value = atol(N.m_str);
//     // ctx.emit(VmOp_push32, value);
//     printf("%ld", value);
// }





// static void visit( CompileCtx &ctx, AstNode *N )
// {
//     N->as_Base.m_symtab = ctx.localTab();

//     switch (N->m_type)
//     {
//         default: return;
//         case Ast_List:      visit_List(ctx, N->as_List);         break;
//         case Ast_Binary:    visit_Binary(ctx, N->as_Binary);     break;
//         case Ast_Prefix:    visit_Prefix(ctx, N->as_Prefix);     break;
//         case Ast_Postfix:   visit_Postfix(ctx, N->as_Postfix);   break;
//         case Ast_Scope:     visit_Scope(ctx, N->as_Scope);       break;
//         case Ast_Assign:    visit_Assign(ctx, N->as_Assign);     break;
//         case Ast_Cond:      visit_Cond(ctx, N->as_Cond);         break;
//         case Ast_Call:      visit_Call(ctx, N->as_Call);         break;
//         case Ast_Return:    visit_Return(ctx, N->as_Return);     break;
//         case Ast_VarDecl:   visit_VarDecl(ctx, N->as_VarDecl);   break;
//         case Ast_FunDecl:   visit_FunDecl(ctx, N->as_FunDecl);   break;
//         case Ast_Var:       visit_Var(ctx, N->as_Var);           break;
//         case Ast_String:    visit_String(ctx, N->as_String);     break;
//         case Ast_Number:    visit_Number(ctx, N->as_Number);     break;
//     }

// }

