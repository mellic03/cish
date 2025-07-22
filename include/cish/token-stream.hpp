#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cish/compile-ctx.hpp>
#include <cish/symbol.hpp>
#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/parse-node.hpp>
#include <cish/prod-node.hpp>



namespace cish
{
    class TokenStream;
}


class cish::TokenStream
{
private:
    Token *m_beg,  *m_end;
    Token *m_prev, *m_curr;

public:
    TokenStream( Token *buf, size_t bufsz );

    bool     isAtEnd();
    Token   *curr() { return m_curr; }
    Token   *end()  { return m_end; }

    Token   *expect( uint32_t type, const char *fmt, ... );
    Token   *consume( uint32_t type, const char *fmt, ... );
    Token   *consume( uint32_t type );
    Token   *advance();
    Token   *check( uint32_t type );
    uint32_t peek( int offset=0 );

    Token   *save();
    void     restore( Token* );

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

    template <typename... Args>
    Token *check( uint32_t type, Args... rest )
    {
        Token *tok = check(type);
        if (tok) return tok;
        return check(rest...);
    }
};

