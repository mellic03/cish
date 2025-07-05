#pragma once
#include <cish/type.hpp>
#include <cish/token.hpp>
// #include <cish/node.hpp>
#include <linkedlist.hpp>


struct AstVisitor;

struct AstNode: knl::LinkedListNode
{
    AstNode() {  }
    virtual void accept( AstVisitor* ) = 0;
};

struct AstRoot;
struct AstGroup;
struct AstCond;
struct AstBinary;
struct AstPrefix;
struct AstPostfix;
struct AstLeaf;

struct AstDecl;
struct AstAssign;
struct AstVariable;
struct AstFunction;
struct AstFunctionCall;
struct AstDeclType;
struct AstNumber;
struct AstReturn;





struct AstVisitor
{
    virtual void visit( AstNode *N ) final { N->accept(this); }; // { N->accept(this); };

    virtual void visitGroup( AstGroup* )  {  };
    virtual void visitCond( AstCond* )  {  };
    virtual void visitBinary( AstBinary* )  {  };
    virtual void visitPrefix( AstPrefix* )  {  };
    virtual void visitPostfix( AstPostfix* )  {  };
    virtual void visitLeaf( AstLeaf* )  {  };
    virtual void visitDecl( AstDecl* )  {  };

    virtual void visitAssign( AstAssign* ) {  };
    virtual void visitVariable( AstVariable* ) {  };
    virtual void visitFunction( AstFunction* ) {  };
    virtual void visitFunctionCall( AstFunctionCall* ) {  };
    virtual void visitDeclType( AstDeclType* ) {  };
    virtual void visitNumber( AstNumber* ) {  };
    virtual void visitReturn( AstReturn* ) {  };
};


struct AstPrint: AstVisitor
{
    virtual void visitGroup( AstGroup *N ) final;
    virtual void visitCond( AstCond *N ) final;
    virtual void visitBinary( AstBinary *N ) final;
    virtual void visitPrefix( AstPrefix *N ) final;
    virtual void visitPostfix( AstPostfix *N ) final;
    virtual void visitDecl( AstDecl *N ) final;
    virtual void visitAssign( AstAssign *N ) final;
    virtual void visitVariable( AstVariable *N ) final;
    virtual void visitDeclType( AstDeclType* ) final;
    virtual void visitNumber( AstNumber* ) final;
    virtual void visitFunctionCall( AstFunctionCall* ) final;
    virtual void visitReturn( AstReturn *N ) final;

};


struct AstExec: AstVisitor
{
    int64_t *m_top = new int64_t[512];
    int64_t m_rax, m_rbx, m_rcx, m_rdx;

    virtual void visitGroup( AstGroup *N ) final;
    virtual void visitCond( AstCond* ) final;
    virtual void visitBinary( AstBinary *N ) final;
    virtual void visitPrefix( AstPrefix *N ) final;
    virtual void visitPostfix( AstPostfix *N ) final;
    virtual void visitDecl( AstDecl *N ) final;
    virtual void visitAssign( AstAssign *N ) final;
    virtual void visitVariable( AstVariable *N ) final;
    virtual void visitDeclType( AstDeclType* ) final;
    virtual void visitNumber( AstNumber* ) final;
    virtual void visitReturn( AstReturn *N ) final;
};




struct AstGroup: public AstNode, knl::LinkedList<AstNode>
{
    template <typename... Args>
    AstGroup( Args... args )
    :   AstNode() { (push(args), ...); }

    bool m_woop = true;

    virtual void accept( AstVisitor *V ) override { V->visitGroup(this); };

private:


};


// struct AstRoot: AstGroup
// {
//     virtual void accept( AstVisitor *V ) final { V->visitRoot(this); };
// };


struct AstCond: AstNode
{
    cish::Token *m_tok;
    AstNode *m_expr, *m_body;

    AstCond( cish::Token *tok, AstNode *expr, AstNode *body )
    :   m_tok(tok), m_expr(expr), m_body(body) {  }

    virtual void accept( AstVisitor *V ) final { V->visitCond(this); };
};


struct AstBinary: AstNode
{
    cish::Token *m_tok;
    AstNode *m_lhs, *m_rhs;
    AstBinary( AstNode *lhs, cish::Token *tok, AstNode *rhs )
    :   m_tok(tok), m_lhs(lhs), m_rhs(rhs) {  }

    virtual void accept( AstVisitor *V ) final { V->visitBinary(this); };
};


struct AstPrefix: AstNode
{
    cish::Token *m_tok;
    AstNode *m_rhs;

    AstPrefix( cish::Token *tok, AstNode *rhs )
    :   m_tok(tok), m_rhs(rhs) {  }

    virtual void accept( AstVisitor *V ) final { V->visitPrefix(this); };
};


struct AstPostfix: AstNode
{
    cish::Token *m_tok;
    AstNode *m_lhs;

    AstPostfix( AstNode *lhs, cish::Token *tok )
    :   m_tok(tok), m_lhs(lhs) {  }

    virtual void accept( AstVisitor *V ) final { V->visitPostfix(this); };
};


struct AstDecl: AstNode
{
    cish::Token *m_tok;
    AstNode *m_target;
    AstDecl( cish::Token *tok, AstNode *target )
    :   m_tok(tok), m_target(target) {  }

    virtual void accept( AstVisitor *V ) final { V->visitDecl(this); };
};



struct AstAssign: AstNode
{
    AstNode *m_dst, *m_expr;
    AstAssign( AstNode *dst, AstNode *expr )
    :   m_dst(dst), m_expr(expr) {  }

    virtual void accept( AstVisitor *V ) final { V->visitAssign(this); };
};



struct AstVariable: AstNode
{
    // cish::Token *m_type;
    // cish::Token *m_idnt;
    const char *m_symkey;
    // AstVariable( cish::Token *type, cish::Token *idnt )
    // :   m_type(type), m_idnt(idnt), m_symkey(idnt->lexeme) {  }
    AstVariable( cish::Token *idnt )
    :   m_symkey(idnt->lexeme) {  }

    virtual void accept( AstVisitor *V ) final { V->visitVariable(this); };
};


struct AstSignature
{
    AstDeclType *m_ret;
    AstGroup    *m_args;
    AstSignature() {  };
};


struct AstFunction: AstNode
{
    cish::Token  *m_name;
    AstSignature *m_sig;
    AstNode      *m_body;
    AstFunction( cish::Token *name, AstSignature *sig, AstNode *body )
    :   m_name(name), m_sig(sig), m_body(body) {  }

    virtual void accept( AstVisitor *V ) final { V->visitFunction(this); };
};

struct AstFunctionCall: AstNode
{
    const char *m_name;
    AstNode *m_args;
    AstFunctionCall( cish::Token *name, AstNode *args )
    :   m_name(name->lexeme), m_args(args) {  }

    virtual void accept( AstVisitor *V ) final { V->visitFunctionCall(this); };
};


struct AstDeclType: AstNode
{
    cish::Token *m_tok;
    AstDeclType( cish::Token *tok )
    :   m_tok(tok) {  }

    virtual void accept( AstVisitor *V ) final { V->visitDeclType(this); };
};



#include <stdlib.h>
struct AstNumber: AstNode
{
    // cish::Token *m_tok;
    // const char *m_symkey;
    int64_t m_data;

    AstNumber()
    :   m_data(0) {  }
    AstNumber( cish::Token *tok )
    :   m_data(atol(tok->lexeme)) {  }

    // :   m_tok(tok), m_symkey(tok->lexeme) {  }

    virtual void accept( AstVisitor *V ) final { V->visitNumber(this); };
};


struct AstReturn: AstNode
{
    AstNode *m_expr;
    AstReturn( AstNode *expr )
    :   m_expr(expr) {  }

    virtual void accept( AstVisitor *V ) final { V->visitReturn(this); };
};

