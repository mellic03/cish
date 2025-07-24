#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cish/compile-ctx.hpp>
#include <cish/symtab.hpp>
#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/parse-node.hpp>
#include <cish/prod-node.hpp>
#include <util/tuple.hpp>


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
    uint32_t peek( int offset=0 );

    Token   *save();
    void     restore( Token* );


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

    TokenStream &match1( uint32_t A )
    {
        auto *state = save();
        if (peek() == A) advance();
        else             restore(state);
        return *this;
    }

    auto match2( uint32_t typeA, uint32_t typeB )
    {
        auto *state = save();
        if (Token *tokA = match(typeA))
            if (Token *tokB = match(typeB))
                return nonstd::make_pair(tokA, tokB);
        restore(state);
        Token *tok = nullptr;
        return nonstd::make_pair(tok, tok);
    }

    auto match3( uint32_t typeA, uint32_t typeB, uint32_t typeC )
    {
        auto *state = save();
        if (Token *tokA = match(typeA))
            if (Token *tokB = match(typeB))
                if (Token *tokC = match(typeC))
                    return nonstd::make_triple(tokA, tokB, tokC);
        restore(state);
        Token *tok = nullptr;
        return nonstd::make_triple(tok, tok, tok);
    }


    template <typename... Types>
    Token *matchSEQ( uint32_t type, Types... rest )
    {
        auto *state = save();

        if (match(type))
            if (matchSEQ(rest...))
                return state;
        restore(state);
        return nullptr;
    }

    template <typename... Types>
    Token *matchSEQ( uint32_t type ) { return match(type); }


    template <typename... Types>
    bool checkOR( uint32_t type, Types... rest ) { return _checkOR(0, type, rest...); }

    template <typename... Types>
    bool checkSEQ( uint32_t type, Types... rest ) { return _checkSEQ(0, type, rest...); }



    // template <typename... Args>
    // Token *check( uint32_t type, Args... rest )
    // {
    //     Token *tok = check(type);
    //     if (tok) return tok;
    //     return check(rest...);
    // }


private:
    bool _check( uint32_t type, int offset )
    {
        return (type == peek(offset));
    }

    template <typename... Types>
    bool _checkOR( int off, uint32_t type, Types... rest )
    {
        if (_check(type, off)) return true;
        return _checkOR(off+1, rest...);
    }

    bool _checkOR( int off, uint32_t type )
    {
        return _check(type, off);
    }


    template <typename... Types>
    bool _checkSEQ( int off, uint32_t type, Types... rest )
    {
        if (!_check(type, off))         return false;
        if (!_checkSEQ(off+1, rest...)) return false;
        return true;
    }

    bool _checkSEQ( int off, uint32_t type )
    {
        return _check(type, off);
    }

};

