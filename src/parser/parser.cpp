#include "parser.hpp"
#include <cish/node.hpp>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define PARSE_ERROR(msg_) { printf("PARSE ERROR: %s\n", msg_); assert(0); }



void cish::Parser::emit_assign()
{
    Token *tok = match(Type::Identifier);
    Node *lhs = new NodeIdentifier(tok);
    tok = consume(Type::Equal, "Expect \"=\" after identifier");
    emit(new NodeBinary(lhs, tok, parseExpr()));
}


void cish::Parser::emit_decl()
{
    consume(Type::KwdLet, "Expect \"let\"");

    Token *tok = consume(Type::Identifier, "Expect identifier after \"let\"");
    Node  *lhs = new NodeIdentifier(tok);

    // DataPrimitive::as_type<int64_t>("i64")
    emit(new NodeDecl(tok->lexeme, "i64"));

    if (peek()->type == Type::Equal)
    {
        retreat();
        emit_assign();
    }
}




cish::Node*
cish::Parser::createParseTree( Token *tokbuf, size_t tokcount )
{
    m_start = tokbuf;
    m_end   = m_start + tokcount;
    m_prev  = nullptr;
    m_curr  = m_start;

    m_root = new NodeGrouping();

    while (!isAtEnd())
    {
        switch (peek()->type)
        {
            default: fprintf(stderr, "idk wtf to do here\n"); assert(0); break;
            case Type::SemiColon:  advance();     break;
            case Type::Identifier: emit_assign(); break;
            case Type::KwdLet:     emit_decl();   break;
        }
    }

    return m_root;
}


// <expr>      ::=  <decl> ";"
//              |   <assign> ";"
//
// <decl>      ::=  "let" IDENTIFIER ("=" <assign>)*
//
// <compEq>    ::=  <comp> (("!="|"==") <comp>)*
// <compRel>   ::=  <term> ((">"|">="|"<"|"<=") <term>)*
// <term>      ::=  <factor> (("-"|"+") <factor>)*
// <unary>     ::=  (("let""!"|"-") <unary>)*
//               |  <primary>
//
// <primary>   ::=  NUMBER | STRING | IDENTIFIER | KEYWORD
//               |  "(" <expr> ")" ;
//

// cish::Node *cish::Parser::parseStatement()
// {
//     // if (match(Type::Identifier))
//     // {
//     //     Node *lhs = new NodeLeaf(m_prev);
//     //     Node *rhs = parseExpr();
//     //     return new NodeBinary(lhs, Type::Equal, rhs);
//     // }

//     if (match(Type::KwdLet, Type::KwdConst))
//     {
//         expect(Type::Identifier, "Expect identifier after (\"let\"|\"const\")");
//         // return new NodeBinary(new NodeLeaf(m_prev), parseExpr());
//         Node *lhs = new NodeLeaf(m_prev->type);
//         Node *rhs = parseExpr();
//         return new NodeBinary(lhs, rhs);
//     }


//     return parseCompEq();
// }


cish::Node *cish::Parser::parseExpr()
{
    return parseCompEqual();
}


cish::Node *cish::Parser::parseCompEqual()
{
    Node *expr = parseCompRelative();
    while (Token *tok = match(Type::Equal, Type::EqualEqual, Type::BangEqual))
        expr = new NodeBinary(expr, tok, parseCompRelative());
    return expr;
}

cish::Node *cish::Parser::parseCompRelative()
{
    Node *expr = parseTerm();
    while (Token *tok = match(Type::Less, Type::Greater, Type::LessEqual, Type::GreaterEqual))
        expr = new NodeBinary(expr, tok, parseTerm());
    return expr;
}

cish::Node *cish::Parser::parseTerm()
{
    Node *expr = parseFactor();        
    while (Token *tok = match(Type::Plus, Type::Minus))
        expr = new NodeBinary(expr, tok, parseFactor());
    return expr;
}


cish::Node *cish::Parser::parseFactor()
{
    Node *expr = parseUnary();
    while (Token *tok = match(Type::Star, Type::Slash))
        expr = new NodeBinary(expr, tok, parseUnary());
    return expr;
}


cish::Node *cish::Parser::parseUnary()
{
    if (Token *tok = match(Type::Bang, Type::Minus))
        return new NodeUnary(tok, parseUnary());
    return parsePrimary();
}


cish::Node *cish::Parser::parsePrimary()
{
    // if (Token *tok = match(Type::KwdLet, Type::KwdConst))
    // {
    //     Node *lhs = new NodeLeaf(tok);
    //     expect(Type::Identifier, "Expect identifier after \"let\"");
    //     Node *rhs = new NodeLeaf(tok);
    //     return new NodeUnary(tok, rhs);
    //     // return new NodeBinary(new NodeLeaf(tok), parseExpr());
    // }

    if (Token *tok = match(Type::Identifier)) return new NodeIdentifier(tok);
    if (Token *tok = match(Type::String))     return new NodeString(tok);
    if (Token *tok = match(Type::Number))     return new NodeNumber(tok);

    if (match(Type::LeftParen))
    {
        Node *expr = parseExpr();
        consume(Type::RightParen, "Expect ')' after expression.");
        return expr;
    }

    printf("[parsePrimary] Should be unreachable\n");
    assert(false);

    return nullptr;
}



void cish::Parser::emit( Node *node )
{
    m_root->m_nodes.insert(node);
}

cish::Token *cish::Parser::expect( uint32_t type, const char *fmt, ... )
{
    if (peek()->type == type)
        return peek();

    va_list vlist;
    va_start(vlist, fmt);
    fprintf(stderr, "[PARSE ERROR] ");
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);

    return nullptr;
}


cish::Token *cish::Parser::consume( uint32_t type, const char *fmt, ... )
{
    if (peek()->type == type)
        return advance();

    va_list vlist;
    va_start(vlist, fmt);
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);

    return nullptr;
}


cish::Token *cish::Parser::advance()
{
    m_prev = m_curr;
    if (m_curr < m_end)
        m_curr++;
    return m_prev;
}


cish::Token *cish::Parser::retreat()
{
    m_prev = m_curr;
    if (m_curr > m_start)
        m_curr--;
    return m_prev;
}


cish::Token *cish::Parser::peek( int offset )
{
    Token *next = m_curr + offset;
    if (m_curr+offset < m_end)
        return m_curr+offset;
    return m_end-1;
};


bool cish::Parser::isAtEnd()
{
    return peek()->type == Type::Eof;
};
