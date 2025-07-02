#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cish/type.hpp>
#include <cish/token.hpp>



namespace cish
{
    class Lexer;
    size_t lexerMain( const char *src, Token *tokbuf, size_t tokbufsz );
}

struct iLexState;


// struct iLexer
// {
//     iLexer( Token *buf, size_t bufsz );

//     Token *tokenize( const char *src );
//     char   advance();
//     // {
//     //     m_prev = m_curr;
//     //     if (m_curr) m_curr++;
//     //     return *m_prev;
//     // };

// protected:
//     void emit();

// private:
//     const char *m_curr;
//     const char *m_prev;
//     char   m_lexbuf[256];
//     char  *m_lextop;
//     Token *m_toktop;
//     Token *m_tokend;
//     size_t m_lineno;
//     size_t m_colno;

// };




class cish::Lexer
{
private:
    const char *m_src;
    const char *m_curr;
    const char *m_prev;
    char   m_lexbuf[256];
    char  *m_lextop;
    Token *m_toktop;
    Token *m_tokend;
    size_t m_lineno;
    size_t m_colno;
    iLexState *getState();
    void reset( const char*, Token*, size_t );

public:
    size_t numStates;
    iLexState **states;
    iLexState *m_cmntState;

    // Lexer( Token *tokbuf, size_t tokbufsz )
    // : toktop(tokbuf), tokend(tokbuf+tokbufsz) {  };

    size_t tokenize( const char *src, Token *tokbuf, size_t tokbufsz );
    bool   isAtEnd();
    void   keep( char );
    char   prev() { return *m_prev; };
    char   peek() { return *m_curr; };
    char   match( const char *brk );
    char   advance();
    char   retreat();
    void   emit( uint32_t type );
    const char *lexbuf() { return m_lexbuf; };

};



