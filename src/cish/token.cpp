#include <cish/token.hpp>
#include <string.h>


cish::Token::Token( uint32_t tp )
:   type(tp)
{
    memset(lexeme, '\0', sizeof(lexeme));
}


cish::Token::Token( uint32_t tp, const char *str )
:   Token(tp)
{
    strncpy(lexeme, str, sizeof(lexeme)-1);
}


cish::Token::Token( uint32_t tp, const char *str, uint32_t line, uint32_t col )
:   Token(tp, str)
{
    lineno = line;
    colno  = col;
}

