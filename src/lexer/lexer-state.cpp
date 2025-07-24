#include <cish/lexer-state.hpp>
#include <cish/lexer.hpp>
#include <cish/keyword.hpp>

#include <string.h>
#include <ctype.h>
#include <iostream>

void iLexState::produce( cish::Lexer &lex )
{
    produce_noemit(lex);
    lex.emit(type());
}

void iLexState::produce_noemit( cish::Lexer &lex )
{
    while (!lex.isAtEnd())
    {
        char ch = lex.peek();
        bool is_inpt = isInput(ch);
        bool is_term = isTerminal(ch);

        if (is_inpt)
            lex.keep(ch);

        if (is_inpt || !is_term)
            lex.advance();

        if (is_term)
            break;

    }
}



void StateNewToken::produce( cish::Lexer &lex )
{
    // while (!lex.isAtEnd())
    // {
    //     char ch = lex.peek();
    //     if (ch!=' ' || ch!='\n')
    //         break;
    //     lex.advance();
    // }
}

// void StateComment::produce( cish::Lexer &lex )
// {
//     while (!lex.isAtEnd() && (lex.peek()=='\n'))
//         lex.advance();
//     lex.emit(Type::None);
// }





#include <stdarg.h>

StateError::StateError( const char *fmt, ... )
{
    va_list vlist;
    va_start(vlist, fmt);
    fprintf(stderr, "[Lexer Error] ");
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
}


bool StateIdentifier::isTrigger( char ch )  { return isInput(ch) && !isdigit(ch); }
bool StateIdentifier::isTerminal( char ch ) { return !isInput(ch); }
bool StateIdentifier::isInput( char ch )    { return isalpha(ch) || isdigit(ch) || ch=='_'; }

void StateIdentifier::produce( cish::Lexer &lex )
{
    produce_noemit(lex);
    auto *buf = lex.lexbuf();

    if      (cish::isKeyword(buf))  lex.emit(cish::getKwdType(buf));
    else                            lex.emit(Type::Identifier);
    // else if (cish::isDataType(buf)) lex.emit(cish::getDataType(buf));
}

// uint32_t StateIdentifier::accept( const char *buf )
// {
//     if (cish::isKeyword(buf))
//         return cish::getKwdType(buf);
//     return Type::Identifier;
// }



bool StateNumber::isTrigger( char ch )  { return isInput(ch); }
bool StateNumber::isTerminal( char ch ) { return !isInput(ch); }
bool StateNumber::isInput( char ch )    { return isdigit(ch); }

bool StateString::isTrigger( char ch )  { return ch == '\"'; }
bool StateString::isTerminal( char ch ) { return ch == '\"'; }
bool StateString::isInput( char ch )    { return ch != '\n'; }
void StateString::produce( cish::Lexer &lex )
{
    int count = 0;

    while (!lex.isAtEnd())
    {
        char ch = lex.advance();

        if (ch == '\"')
        {
            count++;

            if (count == 2)
            {
                lex.emit(Type::String);
                return;
            }
        }
    
        else if (ch == '\n')
        {
            lex.emit(Type::Error);
            return;
        }
    }
}



bool StateOperator::isTrigger( char ch ) { return isInput(ch); }
bool StateOperator::isTerminal( char ch ) { return !isInput(ch); }
bool StateOperator::isInput( char ch )
{
    return (strchr("=!<>+-*/~^|&", ch) != nullptr);
}

void StateOperator::produce( cish::Lexer &lex )
{
    produce_noemit(lex);

    auto *buf = lex.lexbuf();
    if (cish::isOperator(buf))
        lex.emit(cish::getOpType(buf));
    else
        lex.emit(Type::Error);
}

// uint32_t StateOperator::accept( const char *buf )
// {
//     if (cish::isOperator(buf))
//         return cish::getOpType(buf);
//     return Type::Error;
// }



bool StateParen::isInput( char ch )
{
    return (strchr("(){}[]", ch) != nullptr);
}

void StateParen::produce( cish::Lexer &lex )
{
    produce_noemit(lex);
    // lex.keep(lex.advance());
    auto *buf = lex.lexbuf();

    if (strlen(buf) != 1)
    {
        lex.emit(Type::Error);
        return;
    }

    switch (buf[0])
    {
        default:  lex.emit(Type::Error);        break;
        case '(': lex.emit(Type::LeftParen);    break;
        case ')': lex.emit(Type::RightParen);   break;
        case '{': lex.emit(Type::LeftBrace);    break;
        case '}': lex.emit(Type::RightBrace);   break;
        case '[': lex.emit(Type::LeftBracket);  break;
        case ']': lex.emit(Type::RightBracket); break;
    }
}

// uint32_t StateParen::accept( const char *buf )
// {
//     if (strlen(buf) != 1)
//         return Type::Error;

//     switch (buf[0])
//     {
//         default:  return Type::Error;
//         case '(': return Type::LeftParen;
//         case ')': return Type::RightParen;
//         case '{': return Type::LeftBrace;
//         case '}': return Type::RightBrace;
//         case '[': return Type::LeftBracket;
//         case ']': return Type::RightBracket;
//     }
// }

// bool StateOperator::isTrigger( char ch )
// {
//     memset(m_buf, 0, sizeof(m_buf));
//     m_top = m_buf;
//     m_count = 0;

//     if (auto *p = strchr("+-*/=", ch))
//     {
//         *(m_top++) = *p;
//         m_count++;
//         return true;
//     }

//     return false;
// }

// bool StateOperator::isTerminal( char ch )
// {
//     return false;
//     // switch (m_prev)
//     // {
//     //     default:  return false;
//     //     case '+': return (ch == '=');
//     //     case '-': return (ch == '=');
//     //     case '*': return (ch == '=');
//     //     case '/': return (ch == '=');
//     //     case '=': return (ch == '=');
//     // }
//     // return !isInput(ch);
// }

// bool StateOperator::isInput( char ch )
// {
//     size_t count = m_top-m_buf;

//     if (m_count == 0)
//     {
//         return false;
//     }

//     if (m_count == 1)
//     {
//         switch (*(m_top-1))
//         {
//             default:  return false;
//             case '+': return (ch == '=');
//             case '-': return (ch == '=');
//             case '*': return (ch == '=');
//             case '/': return (ch == '=');
//             case '=': return (ch == '=');
//         }
//     }

//     // if (m_top-m_buf > 2)
//     return false;

//     // switch (m_prev)
//     // {
//     //     default:  return false;
//     //     case '+':
//     //     case '-': return ch=='=' || ch==m_prev;
//     //     case '*': return ch == '=';
//     //     case '/': return ch == '=';
//     //     case '=': return ch == '=';
//     // }
// }



