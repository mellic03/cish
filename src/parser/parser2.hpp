#pragma once
#include <stddef.h>
#include <stdint.h>

#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/node.hpp>


namespace cish
{
    class Parser2;
}



class cish::Parser2
{
public: 
    Node *parse( Token *tokbuf, size_t tokcount );

private:
    Token *m_start, *m_end;
    Token *m_prev, *m_curr;
    Token *expect( uint32_t type, const char *fmt, ... );
    Token *consume( uint32_t type, const char *fmt, ... );
    Token *advance();
    Token *peek( int offset=0 );
    bool   isAtEnd();

    template <uint32_t... types>
    Token *match()
    {
        int count = 0;

        ([&]() {
            count += (m_curr->type == types);
        } (), ...);

        if (count > 0)
        {
            advance();
            return m_prev;
        }

        return nullptr;
        // return match(rest...);
    }


    Node *parseStatement();
    Node *parseExpr();
    Node *parseDecl();
    Node *parseAssign();
    // Node *parseComp();
    // Node *parseCompEqual();
    // Node *parseCompRelative();
    // Node *parseTerm();
    // Node *parseFactor();
    // Node *parseUnary();
    // Node *parsePrimary();


};

