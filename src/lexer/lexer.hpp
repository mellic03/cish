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

class cish::Lexer
{
private:
    Token   *m_toktop;
    Token   *m_tokend;
    uint32_t m_emitType;
    bool     m_emitted;
    char     m_strbuf[64];
    char    *m_strtop;
    const char *m_src;


public:
    size_t numStates;
    iLexState **states;
    iLexState *initState;
    iLexState *currState;

    // cishLexer( Token *tokbuf, size_t tokbufsz )
    // :   toktop(tokbuf), tokend(tokbuf+tokbufsz) {  };

    size_t tokenize( const char *src, Token *tokbuf, size_t tokbufsz );
    void   emit( uint32_t type );

};


