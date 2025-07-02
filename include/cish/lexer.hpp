#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cish/token.hpp>


namespace cish
{
    class Lexer;
    struct Token;
}


class cish::Lexer
{
public:
    const char *m_src;
    size_t tokenize( const char *src, Token *buf, size_t bufsz );

    char advance();
    char prev() { return *(m_src-1); }
    char peek() { return *m_src; };
    auto peekstr() { return m_src; };
    bool check( const char *brk );
    char match( const char *brk );
    // bool matchStr( const char* );
    // bool checkBrk( const char* );
    // bool checkStr( const char* );
    bool isAtEnd();
    void emit( uint32_t type );

private:
    const char *m_end;
    Token      *m_out;
    Token      *m_outend;

};

