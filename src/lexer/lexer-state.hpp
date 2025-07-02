#pragma once

#include "lexer.hpp"
#include <cish/type.hpp>
#include <cish/keyword.hpp>
#include <iostream>


using namespace cish;

namespace cish
{
    class Lexer;
}


struct iLexState
{
    virtual uint32_t type() = 0;
    virtual bool isTrigger( char ) = 0;
    virtual bool isInput( char ) = 0;
    virtual bool isTerminal( char ) = 0;
    virtual void produce( cish::Lexer& );

protected:
    virtual void produce_noemit( cish::Lexer& );
    // int m_count;

};



template <uint32_t tp_>
struct LexState: iLexState
{
    using iLexState::iLexState;
    virtual uint32_t type() override { return tp_; };
};


struct StateNewToken: LexState<Type::None>
{
    virtual bool isInput( char ch ) final { return ch==' ' || ch=='\n'; };
    virtual bool isTerminal( char ch ) final { return !isInput(ch); };
    virtual bool isTrigger( char ch ) final { return isInput(ch); };
    virtual void produce( cish::Lexer& ) final;
};


struct StateComment: LexState<Type::None>
{
    virtual bool isTrigger( char ) final {  return false; }
    virtual bool isTerminal( char ch ) final { return ch=='\0' || ch=='\n'; }
    virtual bool isInput( char ch ) final { return true; };
    virtual void produce( cish::Lexer& ) final;
};


struct StateError: LexState<Type::Error>
{
    StateError( const char *fmt, ... );
    virtual bool isInput( char ) final { return false; };
    virtual bool isTerminal( char ) final { return true; };
    virtual bool isTrigger( char ) final { return false; };
};



struct StateIdentifier: LexState<Type::Identifier>
{
    virtual bool isInput( char ) final;
    virtual bool isTerminal( char ) final;
    virtual bool isTrigger( char ) final;
    virtual void produce( cish::Lexer& ) final;
};

struct StateNumber: LexState<Type::Number>
{
    virtual bool isTrigger( char ) final;
    virtual bool isTerminal( char ) final;
    virtual bool isInput( char ) final;
};

struct StateString: LexState<Type::String>
{
    virtual bool isTrigger( char ) final;
    virtual bool isTerminal( char ) final;
    virtual bool isInput( char ) final;
};


template <uint32_t tp_, char ch_>
struct StateSingleChLexeme: LexState<tp_>
{
    virtual bool isTrigger( char ch ) final { return isInput(ch); }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isInput( char ch ) final{ return ch==ch_; }
};

using StateSemiColon = StateSingleChLexeme<Type::SemiColon, ';'>;
// using StatePlus      = StateOneChLexeme<Type::Plus, '+'>;
// using StateMinus     = StateOneChLexeme<Type::Minus, '-'>;
// using StateStar      = StateOneChLexeme<Type::Star, '*'>;

// struct StateSemiColon: LexState<Type::SemiColon>
// {
//     virtual bool isTrigger( char ) final;
//     virtual bool isTerminal( char ) final;
//     virtual bool isInput( char ) final;
// };

struct StateOperator: LexState<Type::Op_BEGIN>
{
    virtual bool isTrigger( char ) final;
    virtual bool isTerminal( char ) final;
    virtual bool isInput( char ) final;
    virtual void produce( cish::Lexer& ) final;
};


struct StateParen: LexState<Type::LeftParen>
{
    virtual bool isTrigger( char ch ) final { return isInput(ch); }
    virtual bool isTerminal( char ch ) final { return isInput(ch); }
    virtual bool isInput( char ) final;
    virtual void produce( cish::Lexer& ) final;
};


// template <uint32_t tp_, char ch_>
// struct StateBasicOneChLexeme: LexState<tp_>
// {
//     using LexState<tp_>::LexState;
//     virtual bool isInput( char ch ) final { return ch==ch_; }
//     virtual bool isTerminal( char ch ) final { return !isInput(ch); }
//     virtual bool isTrigger( const char *s ) final { return isInput(*s); }
// };


// // using StateSemiColon = StateBasicOneChLexeme<Type::SemiColon, ';'>;
// using StateLeftParen = StateBasicOneChLexeme<Type::LeftParen, '('>;
// using StateRightParen = StateBasicOneChLexeme<Type::RightParen, ')'>;
// using StateLeftBrace = StateBasicOneChLexeme<Type::LeftBrace, '{'>;
// using StateRightBrace = StateBasicOneChLexeme<Type::RightBrace, '}'>;
// using StateComma   = StateBasicOneChLexeme<Type::Comma, ','>;
// using StatePeriod  = StateBasicOneChLexeme<Type::Period, '.'>;
// using StateTilde   = StateBasicOneChLexeme<Type::Tilde, '~'>;
// using StateBang    = StateBasicOneChLexeme<Type::Bang, '!'>;


// struct StatePlus: LexState<Type::Plus>
// {
//     virtual bool isInput( char ch ) final { return ch=='+'; }
//     virtual bool isTerminal( char ch ) final { return !isInput(ch); }
//     virtual bool isTrigger( const char *s ) final { return s[0]=='+' && s[1]!='='; }
// };

// struct StateMinus: LexState<Type::Minus>
// {
//     virtual bool isInput( char ch ) final { return ch=='-'; }
//     virtual bool isTerminal( char ch ) final { return !isInput(ch); }
//     virtual bool isTrigger( const char *s ) final { return s[0]=='-' && s[1]!='='; }
// };

// struct StateStar: LexState<Type::Star>
// {
//     virtual bool isInput( char ch ) final { return ch=='*'; }
//     virtual bool isTerminal( char ch ) final { return !isInput(ch); }
//     virtual bool isTrigger( const char *s ) final { return s[0]=='*' && s[1]!='='; }
// };

// // struct StateSlash: LexState<Type::Slash>
// // 
// //     virtual bool isInput( char ch ) final { return ch=='/'; }
// //     virtual bool isTerminal( char ch ) final { return !isInput(ch); }
// //     virtual bool isTrigger( const char *s ) final
// //     {
// //         return s[0]=='/' && s[1]!='/' && s[1]!='=';
// //     }
// // };

// struct StateEqual: LexState<Type::Equal>
// {
//     virtual bool isInput( char ch ) final { return ch=='='; }
//     virtual bool isTerminal( char ch ) final { return !isInput(ch); }
//     virtual bool isTrigger( const char *s ) final { return s[0]=='=' && s[1]!='='; }
// };



// #define STATE_OPX_ASSIGN(name_, op_)\
// struct State##name_: LexState<Type::name_>\
// {\
//     virtual bool isInput( char ch ) final { return ch==op_ || ch=='='; }\
//     virtual bool isTerminal( char ch ) final { return !isInput(ch); }\
//     virtual bool isTrigger( const char *s ) final { return s[0]==op_ && s[1]=='='; }\
// };\

// STATE_OPX_ASSIGN(PlusEqual, '+');
// STATE_OPX_ASSIGN(MinusEqual, '-');
// STATE_OPX_ASSIGN(StarEqual, '*');
// STATE_OPX_ASSIGN(SlashEqual, '/');
// STATE_OPX_ASSIGN(EqualEqual, '=');




