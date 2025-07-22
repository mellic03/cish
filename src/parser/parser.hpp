#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cish/compile-ctx.hpp>
#include <cish/symtab.hpp>
#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/node.hpp>



enum Prec_: uint8_t
{
    Prec_None,
    Prec_Assign,    // =
    Prec_Or,        // |
    Prec_And,       // &
    Prec_Equal,     // == !=
    Prec_Comp,      // < > <= >=
    Prec_AddSub,    // + -
    Prec_MulDiv,    // * /
    Prec_Unary,     // ! -
    Prec_Call,      // . ()
    Prec_Primary
};


namespace cish
{
    class CompileCtx;
    class Parser;
}


class cish::Parser
{
private:
    CompileCtx &m_ctx;

public:
    Token *m_beg,  *m_end;
    Token *m_prev, *m_curr;

    Parser( CompileCtx &ctx, Token *buf, size_t bufsz );

    // Parser( const Parser &P )
    // :   m_ctx(P.m_ctx), m_beg(P.m_beg), m_prev(P.m_prev), m_curr(P.m_curr) {  }

    AstNode *buildPT();
    AstNode *buildAST();

    AstNode *ProdProgram();
    AstNode *ProdStmnt();
    AstNode *ProdExprStmnt();
    AstNode *ProdReturn();
    AstNode *ProdBlock();
    AstNode *ProdTypeName();
    AstNode *ProdCond();
    AstNode *ProdVar();
    AstNode *ProdFun();
    AstNode *ProdFunArgs();
    AstNode *ProdTypeIdnt();
    AstNode *ProdScope();

    AstNode *ProdExpr();
    AstNode *ProdPrecedence();
    AstNode *ProdPostfix();
    AstNode *ProdPrefix();
    AstNode *ProdList();
    AstNode *ProdPrimary();

    Token *expect( uint32_t type, const char *fmt, ... );
    Token *consume( uint32_t type, const char *fmt, ... );
    Token *consume( uint32_t type );
    Token *advance();
    uint32_t peek( int offset=0 );
    Token *check( uint32_t type );
    bool   isAtEnd();
    Token *save();
    void   restore( Token* );

    // template <uint32_t... types>
    template <typename... Args>
    Token *match( uint32_t type, Args... rest )
    {
        if (Token *tok = match(type))
            return tok;
        return match(rest...);
    }

    Token *match( uint32_t type )
    {
        if (m_curr->type == type)
            return advance();
        return nullptr;
    }

    // template <uint32_t... types>
    template <typename... Args>
    Token *check( uint32_t type, Args... rest )
    {
        Token *tok = check(type);
        if (tok) return tok;
        return check(rest...);
    }

    template <typename T>
    AstNode *newNode( const T &nd )
    {
        AstNode *N = (AstNode*)malloc(sizeof(AstNode));
                 N->m_type = T::NodeType();
        T *ptr = new (N->as_bytes) T(nd);
        return N;
    }
};

