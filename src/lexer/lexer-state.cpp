#include "lexer-state.hpp"
#include <cish/keyword.hpp>
#include <string.h>
#include <ctype.h>
#include <iostream>






char *LexState_NewToken::next( char *dst, const char *src )
{
    auto *curr = m_lx.currState;
    if (curr->type() == Type::Comment)
        return curr->next(dst, src);

    if (src[0]==' ' || src[0]=='\n')
        return dst;

    iLexState **sbuf = m_lx.states;
    for (size_t i=0; i<m_lx.numStates; i++)
    {
        if (sbuf[i]!=this && sbuf[i]->isTrigger(src))
        {
            m_lx.currState = sbuf[i];
            if (sbuf[i]->isInput(src[0]))
               return sbuf[i]->next(dst, src);
            return dst;
        }
    }


    // No states triggered by src, throw error
    m_lx.emit(Type::Error);

    return dst;
}



bool LexState_Comment::isInput( char ch )
{ return ch != '\n'; }

bool LexState_Comment::isTerminal( char ch )
{ return ch == '\n'; }

bool LexState_Comment::isTrigger( const char *src )
{ return src[0]=='/' && src[1]=='/'; }

char *LexState_Comment::next( char *dst, const char *src )
{
    if (isTerminal(src[0]))
        m_lx.emit(Type::None);
    return dst;
    // std::cout << "[LexState::next] \'" << src[0] << "\'\t" << cish::TypeToStr(m_lx.currState->type()) << "\n";
}


bool LexState_Keyword::isInput( char ch )
{ return isalpha(ch) || isdigit(ch) || ch=='_'; }

bool LexState_Keyword::isTerminal( char ch )
{ return !isInput(ch); }

bool LexState_Keyword::isTrigger( const char *s ) 
{ return cish::isKeyword(s, &m_kw); }



bool LexState_Identifier::isInput( char ch )
{ return isalpha(ch) || isdigit(ch) || ch=='_'; }

bool LexState_Identifier::isTerminal( char ch )
{ return !isInput(ch); }

bool LexState_Identifier::isTrigger( const char *s )
{ return !cish::isKeyword(s) && !isdigit(*s) && (isalpha(*s) || *s=='_'); }




bool LexState_String::isInput( char ch )
{
    return ch != '\n';
}
bool LexState_String::isTerminal( char ch )
{
    return ch == '\"';
}
bool LexState_String::isTrigger( const char *src )
{
    return src[0] == '\"';
}


