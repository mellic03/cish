#include "parser2.hpp"
#include <cish/node.hpp>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define PARSE_ERROR(msg_) { printf("PARSE ERROR: %s\n", msg_); assert(0); }


cish::Node*
cish::Parser2::parse( Token *tokbuf, size_t tokcount )
{
    m_start = tokbuf;
    m_end   = m_start + tokcount;
    m_prev  = nullptr;
    m_curr  = m_start;

    auto *root = new NodeGrouping();

    while (!isAtEnd())
    {
        Node *node = parseExpr();
        node->print();
        printf("\n");
        root->m_nodes.insert(node);

        while (peek()->type == Type::SemiColon)
        {
            advance();
        }
    }


    return root;
}







class ExprMatcher
{
private:
    ExprMatcher *m_prev, *m_next;

public:
    ExprMatcher( ExprMatcher *prev, ExprMatcher *next )
    :   m_prev(prev), m_next(next)
    {

    }

    virtual bool canFollow( ExprMatcher *prev ) = 0;
    virtual bool canPrecede( ExprMatcher *next ) = 0;

    static bool isValid( ExprMatcher *A, ExprMatcher *B )
    {
        return A->canPrecede(B) && B->canFollow(A);
    }

};






cish::Token *cish::Parser2::expect( uint32_t type, const char *fmt, ... )
{
    if (peek()->type == type)
        return peek();

    va_list vlist;
    va_start(vlist, fmt);
    fprintf(stderr, "[PARSE ERROR] ");
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);

    return nullptr;
}

cish::Token *cish::Parser2::consume( uint32_t type, const char *fmt, ... )
{
    if (peek()->type == type)
        return advance();

    va_list vlist;
    va_start(vlist, fmt);
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);

    return nullptr;
}


cish::Token *cish::Parser2::advance()
{
    m_prev = m_curr++;
    // if (m_curr->type == Type::EOF)
        // PARSE_ERROR("[Parser2::advance] ruh roh");
    return m_curr;
}


cish::Token *cish::Parser2::peek( int offset )
{
    Token *next = m_curr + offset;
    if (m_curr+offset < m_end)
        return m_curr+offset;
    return m_end-1;
};


bool cish::Parser2::isAtEnd()
{
    return peek()->type == Type::Eof;
};
