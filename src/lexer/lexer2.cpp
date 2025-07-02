#include <cish/lexer.hpp>
#include <kstring.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>



void (*currState)(cish::Lexer&);
void stateNewToken(cish::Lexer&);
static char lexBuf[64];
static char *lexTop;
static bool lexeming;

#define LEX_ERROR() { lex.emit(Type::Error); return; }
#define LEX_UNEXPECT() { fprintf(stderr, "Unexpected value \'%c\'\n", lex.prev()); assert(0); }
#define LEX_EMIT(type_) { lex.emit(type_); currState=stateNewToken; return; }
#define LEX_TOSTATE(to_) { currState=to_; return; }
#define LEXTOP_CLEAR() { memset(lexBuf, '\0', sizeof(lexBuf)); lexTop=lexBuf; }
#define LEXEME_STOP() { lexeming=false; }



size_t cish::Lexer::tokenize( const char *src, Token *buf, size_t bufsz )
{
    m_src    = src;
    m_end    = src + strlen(src);
    m_out    = buf;
    m_outend = buf + bufsz;
    currState = stateNewToken;
    LEXTOP_CLEAR();

    while (!isAtEnd())
    {
        currState(*this);
    }

    emit(Type::Eof);
    return m_out - buf;
}


char cish::Lexer::advance()
{
    if (isAtEnd()) return *m_src;
    return *(m_src++);
}

bool cish::Lexer::check( const char *brk )
{
    if (isAtEnd()) return false;
    size_t len = strlen(brk);
    for (size_t i=0; i<len; i++)
        if (peek() == brk[i])
            return true;
    return false;
}

char cish::Lexer::match( const char *brk )
{
    size_t len = strlen(brk);
    for (size_t i=0; i<len; i++)
        if (peek() == brk[i])
            return advance();
    return '\0';
}

bool cish::Lexer::isAtEnd()
{
    return (m_src >= m_end);
}

void cish::Lexer::emit( uint32_t type )
{
    *(m_out++) = Token(type, lexBuf);
    // printf("emit(%s)\n", TypeToStr(type));
    LEXTOP_CLEAR();

    if (type == Type::Error)
        m_src = "\0\0\0\0";
}




// bool cish::Lexer::matchStr( const char *expected )
// {
//     const char *tmp = m_src;
//     const char *str = expected;

//     while (*str)
//     {
//         if (*tmp == '\0')
//             return false;
//         if (*(tmp++) != *(str++))
//             return false;
//     }

//     m_src = tmp;
//     return true;
// }

// bool cish::Lexer::checkBrk( const char *str )
// {
//     if (isAtEnd())
//         return false;

//     size_t len = strlen(str);
//     for (size_t i=0; i<len; i++)
//         if (*m_src == str[i])
//             return true;

//     return false;
// }


// bool cish::Lexer::checkStr( const char *str )
// {
//     size_t len = strlen(str);
//     if (m_end-m_src < len)
//         return false;
//     return (strncmp(m_src, str, len) == 0);
// }











void stateComment( cish::Lexer &lex );
void stateNumber( cish::Lexer &lex );
void stateString( cish::Lexer &lex );
void stateOperator( cish::Lexer &lex );
void stateIdentifier( cish::Lexer &lex );
void stateKeyword( cish::Lexer &lex );


void stateNewToken( cish::Lexer &lex )
{
    using namespace cish;

    while (lex.match(" \n"))
        lex.advance();

    if (isalpha(lex.peek()) || lex.peek()=='_')
        LEX_TOSTATE(stateIdentifier);

    if (lex.match("/"))
    {
        if (lex.match("/"))
            LEX_TOSTATE(stateComment)
        LEX_EMIT(Type::Slash);
    }

    if (lex.check("+-*/"))
        LEX_TOSTATE(stateOperator);

    if (lex.match(";"))
        LEX_EMIT(Type::SemiColon);

    if (lex.match("\""))
        LEX_TOSTATE(stateString)

    if (isdigit(lex.peek()))
        LEX_TOSTATE(stateNumber);


}



void stateComment( cish::Lexer &lex )
{
    using namespace cish;

    while (lex.peek() != '\n')
        lex.advance();

    LEXTOP_CLEAR()
    LEX_TOSTATE(stateNewToken);
}


void stateNumber( cish::Lexer &lex )
{
    using namespace cish;
    LEXTOP_CLEAR();

    char ch = lex.advance();
    while (isdigit(ch))
    {
        *(lexTop++) = ch;
        ch = lex.advance();
    }
    LEX_EMIT(Type::Number);
}



void stateString( cish::Lexer &lex )
{
    printf("[stateString]\n");
    using namespace cish;

    char ch = lex.advance();

    while (ch != '\"')
    {
        *(lexTop++) = ch;
        // if (lex.match("\"")) LEX_EMIT(Type::String);
        // if (lex.match("\n")) LEX_ERROR();
        ch = lex.advance();
    }
}


void stateOperator( cish::Lexer &lex )
{
    printf("[stateOperator]\n");
    using namespace cish;

    char ch = lex.advance();
        *(lexTop++) = ch;
        if (lex.peek() == '=')
            *(lexTop++) = '=';

    switch (ch)
    {

        default:  LEX_UNEXPECT(); break;
        case '+': LEX_EMIT(lex.match("=") ? Type::PlusEqual : Type::Plus); break;
        case '-': LEX_EMIT(lex.match("=") ? Type::MinusEqual : Type::Minus); break;
        case '*': LEX_EMIT(lex.match("=") ? Type::StarEqual : Type::Star); break;
        case '/': LEX_EMIT(lex.match("=") ? Type::SlashEqual : Type::Slash); break;
    }
}



void stateIdentifier( cish::Lexer &lex )
{
    printf("[stateIdentifier]\n");
    using namespace cish;
    LEXTOP_CLEAR();

    char ch = lex.advance();

    while (isalpha(ch) || isdigit(ch) || ch=='_')
    {
        printf("%c\n", ch);
        ch = lex.advance();
    }

    LEX_EMIT(Type::Identifier);
    lexTop++;
}
