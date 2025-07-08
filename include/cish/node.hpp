#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/keyword.hpp>
#include <linkedlist.hpp>


namespace cish
{
    enum Ast_: uint8_t
    {
        Ast_List,
        Ast_Binary,
        Ast_Prefix,
        Ast_Postfix,
        Ast_Scope,
        Ast_Assign,
        Ast_Cond,
        Ast_Call,
        Ast_Return,
        Ast_Type,
        Ast_VarDecl,
        Ast_FunDecl,
        Ast_Var,
        Ast_String,
        Ast_Number,
        Ast_NumTypes,
    };

    template <Ast_ tp_>
    struct AstBase
    {
        static constexpr Ast_ m_type = tp_;
    };

    struct AstList;
    struct AstBinary;
    struct AstPrefix;
    struct AstPostfix;

    struct AstScope;
    struct AstAssign;
    struct AstCond;
    struct AstCall;
    struct AstReturn;

    struct AstType;
    struct AstVarDecl;
    struct AstFunDecl;

    struct AstVar;
    struct AstString;
    struct AstNumber;

    struct AstNode;

}




struct cish::AstList: AstBase<Ast_List>, knl::LinkedList<AstNode>
{
    template <typename... Args>
    AstList( Args... args )
    {
        (push(args), ...);
    }
};


struct cish::AstBinary: AstBase<Ast_Binary>
{
    Token *m_tok;
    AstNode *m_lhs, *m_rhs;

    AstBinary( AstNode *lhs, Token *tok, AstNode *rhs )
    :   m_tok(tok), m_lhs(lhs), m_rhs(rhs) {  }

};


struct cish::AstPrefix: AstBase<Ast_Prefix>
{
    Token *m_tok;
    AstNode *m_rhs;

    AstPrefix( Token *tok, AstNode *rhs )
    :   m_tok(tok), m_rhs(rhs) {  }

};


struct cish::AstPostfix: AstBase<Ast_Postfix>
{
    Token *m_tok;
    AstNode *m_lhs;

    AstPostfix( AstNode *lhs, Token *tok )
    :   m_tok(tok), m_lhs(lhs) {  }

};


struct cish::AstScope: AstBase<Ast_Scope>
{
    AstNode *m_body;

    AstScope( AstNode *body )
    :  m_body(body) {  }
};


struct cish::AstAssign: AstBase<Ast_Assign>
{
    const char *m_var;
    AstNode *m_expr;

    AstAssign( Token *idnt, AstNode *expr )
    :  m_var(idnt->lexeme), m_expr(expr) {  }
};


struct cish::AstCond: AstBase<Ast_Cond>
{
    Token *m_kwd;
    AstNode *m_cond;
    AstNode *m_if, *m_else;

    AstCond( Token *kwd, AstNode *cond, AstNode *if_expr, AstNode *else_expr )
    :  m_kwd(kwd), m_cond(cond), m_if(if_expr), m_else(else_expr) {  }
};


struct cish::AstCall: AstBase<Ast_Call>
{
    const char *m_callee;
    AstNode *m_expr;

    AstCall( Token *callee, AstNode *expr )
    :   m_callee(callee->lexeme), m_expr(expr) {  }
};


struct cish::AstReturn: AstBase<Ast_Return>
{
    AstNode *m_expr;

    AstReturn( AstNode *expr )
    :   m_expr(expr) {  }
};


struct cish::AstType: AstBase<Ast_Type>
{
    const char *m_name;
    size_t m_size;
    size_t m_align;

    AstType( const char *name, size_t size, size_t align )
    :   m_name(name), m_size(size), m_align(align) {  }

    AstType( Token *idnt )
    :   m_name(idnt->lexeme), m_size(8), m_align(8) {  }
};


struct cish::AstVarDecl: AstBase<Ast_VarDecl>
{
    const char *m_typename;
    const char *m_name;

    AstVarDecl( Token *tptok, Token *idtok )
    :   m_typename(tptok->lexeme), m_name(idtok->lexeme) {  }
};


struct cish::AstFunDecl: AstBase<Ast_FunDecl>
{
    const char *m_name;
    const char *m_ret_typename;
    AstNode *m_args;
    AstNode *m_body;

    AstFunDecl( Token *idnt, Token *rtype, AstNode *args, AstNode *body )
    :   m_name(idnt->lexeme),
        m_ret_typename(rtype->lexeme),
        m_args(args),
        m_body(body)
    {

    }
};


struct cish::AstVar: AstBase<Ast_Var>
{
    const char *m_symkey;

    AstVar( Token *tok )
    :   m_symkey(tok->lexeme) {  }
};


struct cish::AstString: AstBase<Ast_String>
{
    const char *m_str;

    AstString( Token *tok )
    :   m_str(tok->lexeme) {  }
};


struct cish::AstNumber: AstBase<Ast_Number>
{
    const char *m_str;

    AstNumber()
    :   m_str("0") {  }

    AstNumber( Token *tok )
    :   m_str(tok->lexeme) {  }
};


struct cish::AstNode: knl::LinkedListNode
{
    Ast_ type;

    union {
        uint8_t     as_bytes[];
        AstList     as_List;
        AstBinary   as_Binary;
        AstPrefix   as_Prefix;
        AstPostfix  as_Postfix;
        AstScope    as_Scope;
        AstAssign   as_Assign;
        AstCond     as_Cond;
        AstCall     as_Call;
        AstReturn   as_Return;
        AstType     as_Type;
        AstVarDecl  as_VarDecl;
        AstFunDecl  as_FunDecl;
        AstVar      as_Var;
        AstString   as_String;
        AstNumber   as_Number;
    };
};


#include <stdlib.h>
#include <new>

namespace cish
{
    // template <typename T, typename... Args>
    // AstNode *allocNode( Args... args )
    // {
    //     auto *N = new AstNode();
    //     auto *t = new (N->as_bytes) T(args...);
    //     N->type = t->m_type;
    //     return N;
    // }

    template <typename T>
    AstNode *newNode( const T &nd )
    {
        AstNode *N = (AstNode*)malloc(sizeof(AstNode));
                 N->type = nd.m_type;

        new (N->as_bytes) T(nd);
        return N;
    }

}