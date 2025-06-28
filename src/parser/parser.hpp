#pragma once
#include <stddef.h>
#include <stdint.h>

#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/node.hpp>


namespace cish
{
    class Parser;
}


class cish::Parser
{
public:
    Node *createParseTree( Token *tokbuf, size_t tokcount );

private:
    Token *m_start, *m_end;
    Token *m_prev, *m_curr;
    NodeGrouping *m_root;

    void emit_assign();
    void emit_decl();

    Node *parseExpr();
    Node *parseCompEqual();
    Node *parseCompRelative();
    Node *parseTerm();
    Node *parseFactor();
    Node *parseUnary();
    Node *parsePrimary();

    void   emit( Node* );
    Token *expect( uint32_t type, const char *fmt, ... );
    Token *consume( uint32_t type, const char *fmt, ... );
    Token *advance();
    Token *retreat();
    Token *peek( int offset=0 );
    bool   isAtEnd();

    // template <uint32_t... types>
    template <typename... Args>
    Token *match( uint32_t type, Args... rest )
    {
        if (m_curr->type == type)
        {
            return advance();
        }

        return match(rest...);
    }

    Token *match()
    {
        return nullptr;
    }

};
