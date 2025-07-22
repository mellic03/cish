#include <cish/compile-pass.hpp>
#include <assert.h>

using namespace cish;


void cish::NodeVisitor::visit( CompileCtx &ctx, ProdNode &N )
{
    // switch (N.as_Tag)
    // {
    //     case ProdTag::List:      visit_List      (ctx, N.as_List);      return;
    //     case ProdTag::Atom:      visit_Atom      (ctx, N.as_Atom);      return;
    //     case ProdTag::Program:   visit_Program   (ctx, N.as_Program);   return;
    //     case ProdTag::Stmnt:     visit_Stmnt     (ctx, N.as_Stmnt);     return;
    //     case ProdTag::StmntList: visit_StmntList (ctx, N.as_StmntList); return;
    //     case ProdTag::Decl:      visit_Decl      (ctx, N.as_Decl);      return;
    //     case ProdTag::Assign:    visit_Assign    (ctx, N.as_Assign);    return;
    //     case ProdTag::Expr:      visit_Expr      (ctx, N.as_Expr);      return;
    //     case ProdTag::VarDecl:   visit_VarDecl   (ctx, N.as_VarDecl);   return;
    //     case ProdTag::FunDecl:   visit_FunDecl   (ctx, N.as_FunDecl);   return;
    //     case ProdTag::DeclList:  visit_DeclList  (ctx, N.as_DeclList);  return;
    //     case ProdTag::Call:      visit_Call      (ctx, N.as_Call);      return;
    //     case ProdTag::CallList:  visit_CallList  (ctx, N.as_CallList);  return;
    //     case ProdTag::Term:      visit_Term      (ctx, N.as_Term);      return;
    // }

    assert((false));
}



