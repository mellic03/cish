#pragma once
#include <stddef.h>
#include <stdint.h>

#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/node.hpp>



namespace cish
{
    class Parser;
}


struct AstNode;


class cish::Parser
{
public:
    AstNode *buildPT( Token* );
    AstNode *buildAST( Token* );


private:
    Token   *m_prev, *m_curr;

    AstNode *ProdProgram();
    AstNode *ProdStmnt();
    AstNode *ProdExprStmnt();
    AstNode *ProdReturn();
    AstNode *ProdBlock();
    AstNode *ProdTypeName();
    AstNode *ProdCond();
    AstNode *ProdDecl();
    AstNode *ProdScope();

    AstNode *ProdExpr();
    AstNode *ProdPrecedence( uint8_t p );
    Token   *ProdOperator( uint8_t p, bool &is_right );
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

};

