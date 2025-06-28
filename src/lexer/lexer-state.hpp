#pragma once

#include "lexer.hpp"
#include <cish/type.hpp>
#include <cish/keyword.hpp>
#include <iostream>


using namespace cish;

struct iLexState
{
    cish::Lexer &m_lx;

    iLexState( cish::Lexer &lx )
    :   m_lx(lx) {  };

    virtual uint32_t type() = 0;
    virtual bool isTrigger( const char* ) = 0;
    virtual bool isInput( char ) = 0;
    virtual bool isTerminal( char ) = 0;
    // virtual char *next( char*, const char* );

    virtual char *next( char *dst, const char *src )
    {
        // std::cout << "\'" << src[0] << "\' ";

        if (isTerminal(src[0])) 
        {
            // std::cout << "emit " << TypeToStr(this->type()) << "\n";
            m_lx.emit(this->type());
        }

        else if (isInput(src[0]))
        {
            // std::cout << "inpt " << "\n";
            *(dst++) = *src;
        }

        else
        {
            m_lx.emit(Type::Error);
        }

        return dst;
    }

};



template <uint32_t tp_>
struct LexState: iLexState
{
    using iLexState::iLexState;
    virtual uint32_t type() override { return tp_; };
};


struct LexState_NewToken: LexState<Type::None>
{
    using LexState::LexState;

    virtual bool isInput( char ) final { return false; };
    virtual bool isTerminal( char ) final { return false; };
    virtual bool isTrigger( const char* ) final { return false; };
    virtual char *next( char*, const char* ) final;
};



struct LexState_Comment: LexState<Type::Comment>
{
    using LexState::LexState;

    virtual bool isInput( char ) final;
    virtual bool isTerminal( char ) final;
    virtual bool isTrigger( const char* ) final;
    virtual char *next( char*, const char* ) final;
};



struct LexState_Keyword: LexState<Type::Keyword>
{
    uint32_t m_kw; // AstKw_
    using LexState::LexState;
    virtual uint32_t type() final { return (uint32_t)m_kw; };
    virtual bool isInput( char ) final;
    virtual bool isTerminal( char ) final;
    virtual bool isTrigger( const char* ) final;
};


struct LexState_Identifier: LexState<Type::Identifier>
{
    using LexState::LexState;
    virtual bool isInput( char ) final;
    virtual bool isTerminal( char ) final;
    virtual bool isTrigger( const char* ) final;
};



template <uint32_t tp_, char ch_>
struct LexState_BasicOneChLexeme: LexState<tp_>
{
    using LexState<tp_>::LexState;
    virtual bool isInput( char ch ) final { return ch==ch_; }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isTrigger( const char *s ) final { return isInput(*s); }
};

struct LexState_SemiColon: LexState<Type::SemiColon>
{
    using LexState::LexState;
    virtual bool isInput( char ch ) final { return ch==';'; }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isTrigger( const char *s ) final { return isInput(*s); }
};

// using LexState_SemiColon = LexState_BasicOneChLexeme<Type::SemiColon, ';'>;
using LexState_LeftParen = LexState_BasicOneChLexeme<Type::LeftParen, '('>;
using LexState_RightParen = LexState_BasicOneChLexeme<Type::RightParen, ')'>;
using LexState_LeftBrace = LexState_BasicOneChLexeme<Type::LeftBrace, '{'>;
using LexState_RightBrace = LexState_BasicOneChLexeme<Type::RightBrace, '}'>;
using LexState_Comma   = LexState_BasicOneChLexeme<Type::Comma, ','>;
using LexState_Period  = LexState_BasicOneChLexeme<Type::Period, '.'>;
using LexState_Tilde   = LexState_BasicOneChLexeme<Type::Tilde, '~'>;
using LexState_Bang    = LexState_BasicOneChLexeme<Type::Bang, '!'>;


struct LexState_Plus: LexState<Type::Plus>
{
    using LexState::LexState;
    virtual bool isInput( char ch ) final { return ch=='+'; }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isTrigger( const char *s ) final { return s[0]=='+' && s[1]!='='; }
};

struct LexState_Minus: LexState<Type::Minus>
{
    using LexState::LexState;
    virtual bool isInput( char ch ) final { return ch=='-'; }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isTrigger( const char *s ) final { return s[0]=='-' && s[1]!='='; }
};

struct LexState_Star: LexState<Type::Star>
{
    using LexState::LexState;
    virtual bool isInput( char ch ) final { return ch=='*'; }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isTrigger( const char *s ) final { return s[0]=='*' && s[1]!='='; }
};

struct LexState_Slash: LexState<Type::Slash>
{
    using LexState::LexState;
    virtual bool isInput( char ch ) final { return ch=='/'; }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isTrigger( const char *s ) final
    {
        return s[0]=='/' && s[1]!='/' && s[1]!='=';
    }
};

struct LexState_Equal: LexState<Type::Equal>
{
    using LexState::LexState;
    virtual bool isInput( char ch ) final { return ch=='='; }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isTrigger( const char *s ) final { return s[0]=='=' && s[1]!='='; }
};



#define STATE_OPX_ASSIGN(name_, op_)\
struct LexState_##name_: LexState<Type::name_>\
{\
    using LexState::LexState;\
    virtual bool isInput( char ch ) final { return ch==op_ || ch=='='; }\
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }\
    virtual bool isTrigger( const char *s ) final { return s[0]==op_ && s[1]=='='; }\
};\

STATE_OPX_ASSIGN(PlusEqual, '+');
STATE_OPX_ASSIGN(MinusEqual, '-');
STATE_OPX_ASSIGN(StarEqual, '*');
STATE_OPX_ASSIGN(SlashEqual, '/');
STATE_OPX_ASSIGN(EqualEqual, '=');



struct LexState_Number: LexState<Type::Number>
{
    using LexState::LexState;
    virtual bool isInput( char ch ) final { return isdigit(ch); }
    virtual bool isTerminal( char ch ) final { return !isInput(ch); }
    virtual bool isTrigger( const char *s ) { return isInput(*s); }
};


struct LexState_String: LexState<Type::String>
{
    using LexState::LexState;

    virtual bool isInput( char ) final;
    virtual bool isTerminal( char ) final;
    virtual bool isTrigger( const char* ) final;
    // virtual char *next( char*, const char* ) final;
};



