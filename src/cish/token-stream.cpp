#include <cish/token-stream.hpp>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

using namespace cish;



cish::TokenStream::TokenStream( Token *buf, size_t bufsz )
:   m_beg(buf), m_end(buf+bufsz),
    m_prev(buf), m_curr(buf)
{

}




bool cish::TokenStream::isAtEnd()
{
    return m_curr->type == Type::Eof;
}


cish::Token *cish::TokenStream::expect( uint32_t type, const char *fmt, ... )
{
    if (m_curr->type == type)
        return m_curr;

    va_list vlist;
    va_start(vlist, fmt);
    fprintf(stderr, "[PARSE ERROR] line %u, col %u\n", m_curr->lineno, m_curr->colno);
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);

    return nullptr;
}


cish::Token *cish::TokenStream::consume( uint32_t type, const char *fmt, ... )
{
    if (m_curr->type == type)
        return advance();

    va_list vlist;
    va_start(vlist, fmt);
    fprintf(stderr, "[PARSE ERROR] line %u, col %u\n", m_curr->lineno, m_curr->colno);
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);

    return nullptr;
}

cish::Token *cish::TokenStream::consume( uint32_t type )
{
    return consume(
        type, "Expected \"%s\", received \"%s\"",
        TypeToStr(type), m_prev->lexeme
    );
}


cish::Token *cish::TokenStream::advance()
{
    if (!isAtEnd())
        m_prev = m_curr++;
    return m_prev;
}


cish::Token *cish::TokenStream::check( uint32_t type )
{
    if (m_curr->type == type)
        return m_curr;
    return nullptr;
};


uint32_t cish::TokenStream::peek( int offset )
{
    return (m_curr+offset)->type;
}


Token *cish::TokenStream::save()
{
    return m_curr;
}

void cish::TokenStream::restore( Token *tok )
{
    // printf("[restore] [%lu, %lu] <--", (m_prev-m_beg), (m_curr-m_beg));
    if (tok > m_beg)
        m_prev = tok-1;
    m_curr = tok;
    // printf(" [%lu, %lu]\n", (m_prev-m_beg), (m_curr-m_beg));
}



