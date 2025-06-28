#include "lexer.hpp"
#include "lexer-state.hpp"
#include <string.h>
#include <ctype.h>
#include <iostream>


size_t cish::Lexer::tokenize( const char *src, Token *tokbuf, size_t tokbufsz )
{
    m_toktop   = tokbuf;
    m_tokend   = tokbuf + tokbufsz;
    m_emitType = Type::None;
    m_emitted  = false;

    currState = initState;

    memset(m_strbuf, '\0', sizeof(m_strbuf));
    m_strtop = m_strbuf;
    m_src = src;

    size_t lineNo = 0;
    size_t colNo  = 0;

    while (*m_src && m_toktop<m_tokend)
    {
        if (*m_src == '\n')
        {
            lineNo++;
            colNo = 0;
        }

        m_strtop = currState->next(m_strtop, m_src);
    
        if (!m_emitted)
        {
            colNo++;
            m_src++;
            continue;
        }

        if (m_emitType == Type::Error)
        {
            // std::cout << "[cish::Lexer::tokenize] Error on line " << lineNo << ", col " << colNo << "\n";
            break;
        }

        else if (m_emitType != Type::None)
        {
            // std::cout << "[cish::Lexer::emit] " << cish::TypeToStr(m_emitType) << "\n\n";
            *(m_toktop++) = Token(m_emitType, m_strbuf);
        }

        memset(m_strbuf, '\0', sizeof(m_strbuf));
        m_strtop = m_strbuf;

        m_emitted = false;
        currState = initState;
    }

    *(m_toktop++) = Token(Type::Eof, "");

    return m_toktop - tokbuf;
}


void cish::Lexer::emit( uint32_t type )
{
    m_emitType = type;
    m_emitted  = true;
}







size_t cish::lexerMain( const char *src, Token *tokbuf, size_t tokbufsz )
{
    static cish::Lexer glexer;

    // static auto *state_newtoken = new LexState_NewToken(glexer);
    static iLexState *states[] = {
        new LexState_NewToken(glexer),
        new LexState_Comment(glexer),
        new LexState_Keyword(glexer),
        new LexState_Identifier(glexer),
        new LexState_SemiColon(glexer),

        new LexState_Bang(glexer),
        new LexState_Plus(glexer),
        new LexState_Minus(glexer),
        new LexState_Star(glexer),
        new LexState_Slash(glexer),
        new LexState_Equal(glexer),

        new LexState_PlusEqual(glexer),
        new LexState_MinusEqual(glexer),
        new LexState_StarEqual(glexer),
        new LexState_SlashEqual(glexer),
        new LexState_EqualEqual(glexer),

        new LexState_LeftParen(glexer),
        new LexState_RightParen(glexer),
        new LexState_Comma(glexer),
        new LexState_Period(glexer),

        new LexState_Number(glexer),
        new LexState_String(glexer),
    };

    glexer.numStates = sizeof(states) / sizeof(states[0]);
    glexer.states    = states;
    glexer.initState = states[0];
    return glexer.tokenize(src, tokbuf, tokbufsz);
}

