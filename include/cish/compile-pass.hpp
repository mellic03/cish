#pragma once

#include <cish/prod-node.hpp>


namespace cish
{
    class CompileCtx;

    struct NodeVisitor
    {
        void visit( CompileCtx&, ProdNode &N );

        // virtual void visit_List      ( CompileCtx&, List& ) = 0;
        // virtual void visit_Atom      ( CompileCtx&, Atom& ) = 0;
        // virtual void visit_Program   ( CompileCtx&, Program& ) = 0;
        // virtual void visit_Stmnt     ( CompileCtx&, Stmnt& ) = 0;
        // virtual void visit_StmntList ( CompileCtx&, StmntList& ) = 0;
        // virtual void visit_Decl      ( CompileCtx&, Decl& ) = 0;
        // virtual void visit_Assign    ( CompileCtx&, Assign& ) = 0;
        // virtual void visit_Expr      ( CompileCtx&, Expr& ) = 0;
        // virtual void visit_VarDecl   ( CompileCtx&, VarDecl& ) = 0;
        // virtual void visit_FunDecl   ( CompileCtx&, FunDecl& ) = 0;
        // virtual void visit_DeclList  ( CompileCtx&, DeclList& ) = 0;
        // virtual void visit_Call      ( CompileCtx&, Call& ) = 0;
        // virtual void visit_CallList  ( CompileCtx&, CallList& ) = 0;
        // virtual void visit_Term      ( CompileCtx&, Term& ) = 0;
    };
}

