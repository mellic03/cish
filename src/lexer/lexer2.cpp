// #include <cish/lexer.hpp>
// #include <kstring.h>
// #include <ctype.h>



// void (*currState)(cish::Lexer&);
// void stateNewToken(cish::Lexer&);

// size_t cish::Lexer::tokenize( const char *src, Token *buf, size_t bufsz )
// {
//     m_curr   = src;
//     m_end    = src + strlen(src);
//     m_out    = buf;
//     m_outend = buf + bufsz;
//     currState = stateNewToken;

//     while (!isAtEnd())
//     {
//         currState(*this);

//         // switch (*m_curr)
//         // {
//         //     default: break;
//         // }
//     }
// }


// char cish::Lexer::advance()
// {
//     return *(m_curr++);
// }


// char cish::Lexer::peek( int offset )
// {
//     if (isAtEnd())
//         return '\0';
//     return *(m_curr+1);
// }


// bool cish::Lexer::match( char expected )
// {
//     if (isAtEnd()) return false;
//     if (*m_curr != expected) return false;
//     m_curr++;
//     return true;
// }


// bool cish::Lexer::matchStr( const char *expected )
// {
//     const char *tmp = m_curr;
//     const char *str = expected;

//     while (*str)
//     {
//         if (*tmp == '\0')
//             return false;
//         if (*(tmp++) != *(str++))
//             return false;
//     }

//     m_curr = tmp;
//     return true;
// }


// bool cish::Lexer::check( char ch )
// {
//     if (isAtEnd()) return false;
//     return (peek() == ch);
// }


// bool cish::Lexer::checkBrk( const char *str )
// {
//     if (isAtEnd())
//         return false;

//     size_t len = strlen(str);
//     for (size_t i=0; i<len; i++)
//         if (*m_curr == str[i])
//             return true;

//     return false;
// }


// bool cish::Lexer::checkStr( const char *str )
// {
//     size_t len = strlen(str);
//     if (m_end-m_curr < len)
//         return false;
//     return (strncmp(m_curr, str, len) == 0);
// }


// bool cish::Lexer::isAtEnd()
// {
//     return (*m_curr == '\0') || (m_curr >= m_end);
// }


// void cish::Lexer::emit( uint32_t type )
// {
//     *(m_out++) = Token(type);

//     if (type == Type::Error)
//         m_curr = "\0\0\0\0";
// }











// #define LEX_ERROR() { lex.emit(Type::Error); return; }
// #define LEX_EMIT(type_) { lex.emit(type_); return; }
// #define LEX_TOSTATE(to_) { currState=to_; return; }

// void stateComment( cish::Lexer &lex );
// void stateIdentifier( cish::Lexer &lex );
// void stateNumber( cish::Lexer &lex );
// void stateString( cish::Lexer &lex );
// void stateOperator( cish::Lexer &lex );


// void stateNewToken( cish::Lexer &lex )
// {
//     using namespace cish;

//     while (lex.checkBrk(" \n"))
//         lex.advance();

//     if (lex.matchStr("//"))
//         LEX_TOSTATE(stateComment)

//     if (lex.checkBrk("+-*/"))
//         LEX_TOSTATE(stateOperator);

//     if (lex.match(';'))
//         LEX_EMIT(Type::SemiColon);

//     if (lex.match('\"'))
//         LEX_TOSTATE(stateString)

//     if (isdigit(lex.peek()))
//     {
//         LEX_TOSTATE(stateNumber);
//     }


// }



// void stateComment( cish::Lexer &lex )
// {
//     using namespace cish;

//     while (true)
//     {
//         if (lex.match('\n'))
//             LEX_TOSTATE(stateNewToken);
//         lex.advance();
//     }

//     lex.emit(Type::Number);
// }


// void stateIdentifier( cish::Lexer &lex )
// {
//     using namespace cish;

//     while (!lex.isAtEnd())
//     {
//         lex.advance();
//     }

//     lex.emit(Type::Number);
// }


// void stateNumber( cish::Lexer &lex )
// {
//     using namespace cish;
//     while (isdigit(lex.peek()))
//         lex.advance();
//     lex.emit(Type::Number);
// }



// void stateString( cish::Lexer &lex )
// {
//     using namespace cish;

//     if (!lex.match('\"'))
//         LEX_ERROR();
//     lex.advance();
    
//     while (lex.match('\"'))
//     {
//         if (lex.peek() == '\n')
//             LEX_ERROR();
//         lex.advance();
//     }
// }


// void stateOperator( cish::Lexer &lex )
// {
//     using namespace cish;

//     if (lex.matchStr("+=")) LEX_EMIT(Type::PlusEqual);
//     if (lex.match   ('+'))  LEX_EMIT(Type::Plus);

//     if (lex.match('-'))
//     {

//     }

//     if (lex.match('*'))
//     {

//     }

//     if (lex.match('/'))
//     {

//     }

//     switch (lex.advance())
//     {
//         case '+':
//             if (lex.match('='))
//             if (lex.peek() == '=')
//                 LEX_EMIT(Type::PlusEqual)
//             break;
//         case '-': break;
//         case '*': break;
//         case '/': break;
//     }
//     if (lex.checkStr("+="))
//     while (isdigit(lex.peek()))
//         lex.advance();
//     lex.emit(Type::Number);
// }


