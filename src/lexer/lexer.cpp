#include <cish/lexer.hpp>
#include <cish/lexer-state.hpp>
#include <cish/keyword.hpp>
#include <string.h>
#include <kstring.h>
#include <ctype.h>
#include <iostream>




static bool is_comment( const char *src )
{
    if (!(src[0] && src[1]))
        return false;
    return (src[0]=='/') && (src[1]=='/');
}

static const char *skip_comment( const char *src )
{
    while (*src && *src != '\n')
        src++;
    return src;
}


iLexState *cish::Lexer::getState()
{
    while (peek()==' ')
    {
        advance();
    }

    if (*m_curr=='/' && *(m_curr+1)=='/')
    {
        while (peek() != '\n')
        {
            advance();
        }
    }

    char ch = peek();

    for (size_t i=0; i<numStates; i++)
    {
        if (states[i]->isTrigger(ch))
        {
            // printf("\'%c\' trig %s\n", ch, TypeToStr(states[i]->type()));
            return states[i];
        }
    }

    return nullptr;
    // return new StateError("No state triggered by \'%c\'\n", ch);
}



size_t cish::Lexer::tokenize( const char *src, Token *tokbuf, size_t tokbufsz )
{
    reset(src, tokbuf, tokbufsz);

    while (!isAtEnd())
    {
        iLexState *S = nullptr;

        if (S = getState())
            S->produce(*this);
        else
            advance();
    }

    emit(Type::Eof);

    return m_toktop - tokbuf;
}


void cish::Lexer::reset( const char *src, Token *tokbuf, size_t tokbufsz )
{
    memset(m_lexbuf, '\0', sizeof(m_lexbuf));
    m_lextop  = m_lexbuf;
    m_toktop  = tokbuf;
    m_tokend  = tokbuf + tokbufsz;
    m_lineno  = 0;
    m_colno   = 0;
    m_src     = src;
    m_prev    = src;
    m_curr    = src;
}


bool cish::Lexer::isAtEnd()
{
    return !(*m_curr && m_toktop<m_tokend);
}


void cish::Lexer::keep( char ch )
{
    *(m_lextop++) = ch;
    if (m_lextop-m_lexbuf >= sizeof(m_lexbuf))
        *(--m_lextop) = '\0';
}



char cish::Lexer::match( const char *brk )
{
    for (size_t i=0; i<strlen(brk); i++)
        if (*m_curr == brk[i])
            return advance();
    // if (strchr(brk, peek()))
        // return advance();
    return '\0';
}


char cish::Lexer::advance()
{
    m_prev = m_curr;
    if (!isAtEnd())
        m_curr++;

    m_colno++;

    if (*m_prev == '\n')
    {
        m_lineno++;
        m_colno = 0;
    }

    return *m_prev;

};

char cish::Lexer::retreat()
{
    m_prev = m_curr;
    if (m_src < m_curr)
        m_curr--;
    return *m_prev;

};



void cish::Lexer::emit( uint32_t type )
{
    if (type == Type::Eof)
        m_lexbuf[0] = '\0';

    if (type != Type::None)
        *(m_toktop++) = Token(type, m_lexbuf, m_lineno, m_colno);

    // printf("\'%c\' emit [%s] \"%s\" \n", *m_curr, TypeToStr(type), m_lexbuf);

    memset(m_lexbuf, '\0', sizeof(m_lexbuf));
    m_lextop = m_lexbuf;
}



size_t cish::lexerMain( const char *src, Token *tokbuf, size_t tokbufsz )
{
    static cish::Lexer glexer;

    static iLexState *states[] = {
        // glexer.initState,
        new StateIdentifier(),
        new StateNumber(),
        new StateString(),
        new StateOperator(),
        new StateParen(),
        new StateSemiColon(),
        new StateComma(),
    };

    glexer.numStates = sizeof(states) / sizeof(states[0]);
    glexer.states    = states;
    return glexer.tokenize(src, tokbuf, tokbufsz);
}

