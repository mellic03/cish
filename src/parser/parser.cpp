#include "parser.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

using namespace cish;



/*
    <program>   ::= <stmnt>*
    <stmnt>     ::= (<decl> | <assign> | <expr>) ";"

    <decl>      ::= <vardecl> | <fundecl>
    <vardecl>   ::= type idnt
    <fundecl>   ::= type idnt "(" <argdecl>? ")"
    <argdecl>   ::= type idnt ("," <argdecl>)*

    <assign>    ::= idnt "=" <expr>

    <call>      ::= <primary> "(" <args>? ")"
    <args>      ::= <expr> ("," <expr>)*

    <expr>      ::= <term> ((/|*|-|+) <expr>)*
    <term>      ::= idnt | string | number | <expr>
*/

cish::Parser::Parser( CompileCtx &ctx, Token *buf, size_t bufsz )
:   m_ctx(ctx),
    m_beg(buf), m_end(buf+bufsz),
    m_prev(buf), m_curr(buf)
{

}


AstNode *cish::Parser::buildPT()
{
    // for (int i=1; i<ctx->m_symtabs.size(); i++)
    //     ctx->m_symtabs[i]->clear();
    auto *root = newNode(AstList());
    // root->m_symtab = &(m_ctx->getGlobal());

    while (!isAtEnd())
    {
        root->as_List.push(ProdStmnt());
    }

    // AstPrint AP;
    // AP.visit(root);

    return root;
}


AstNode *cish::Parser::buildAST()
{
    AstNode *PT = buildPT();
    AstNode *AST = PT;
    return AST;
}


/*
    <factor>  ::= <atom> | <decl> | <expr>
    
    <program> ::= <stmnt>*

    <stmnt>   ::= <exprst>
                | <cond>

    <exprst>  ::= <kw_decl> ("=" <expr>)* ";"
                | "return" <expr>* ";"
                | <expr>* ";"

    <kw_decl> ::= "let" | "const"

    <cond>    ::= ("if"|"while") "("<expr>")" "{" <stmnt>* "}"

    <expr>    ::= <primary> (OPERATOR <expr>)*
    <call>    ::= <primary> "(" <list>? ")"
    <list>    ::= <expr> ("," <expr>)*
    <primary> ::= IDENTIFIER | STRING | NUMBER
    <assign>  ::= IDENTIFIER "=" <expr>

*/




AstNode *cish::Parser::ProdProgram()
{
    AstNode *prog = nullptr;

    return prog;
}



static bool is_typename( uint32_t tp )
{
    return (Type::DataType_BEGIN <= tp && tp < Type::DataType_END);
}

AstNode *cish::Parser::ProdStmnt()
{
    if (check(Type::KwdLet))
        return ProdVar();
    if (check(Type::KwdFunc))
        return ProdFun();
    if (check(Type::KwdReturn))
        return ProdReturn();
    if (check(Type::KwdIf, Type::KwdWhile))
        return ProdCond();
    return ProdExprStmnt();
}



AstNode *cish::Parser::ProdVar()
{
    Token *kwtok = consume(Type::KwdLet);

    if (!is_typename(peek()))
        consume(Type::Error, "Expected typename after \"let\"");

    Token  *tptok = advance();
    Token  *idtok = consume(Type::Identifier, "Expected <identifier> after \"let %s\"", tptok->lexeme);
    AstNode *idnt = newNode(AstVar(idtok));
    AstNode *decl = newNode(AstVarDecl(tptok, idtok));

    if (match(Type::SemiColon))
        return decl;

    consume(Type::Equal, "Expected \"=\" after \"let <identifier>\"");
    AstNode *expr = newNode(AstAssign(idtok, ProdExprStmnt()));

    return newNode(AstList(decl, expr));
}


AstNode *cish::Parser::ProdFun()
{
    Token *kwtok = consume(Type::KwdFunc);

    if (!is_typename(peek()))
        consume(Type::Error, "Expected typename after \"func\"");

    Token *rtyp = advance();
    Token *idnt = consume(Type::Identifier, "Expected <identifier> after \"func %s\"", rtyp->lexeme);


    m_ctx.pushTab(); // ------------------------------------------------------
    consume(Type::LeftParen);
    AstNode *args = ProdFunArgs();

    consume(Type::LeftBrace);
    AstNode *body = newNode(AstList());
    while (!match(Type::RightBrace))
        body->as_List.push(ProdStmnt());
    // consume(Type::RightBrace);
    m_ctx.popTab(); // -------------------------------------------------------

    AstNode *func = newNode(AstFunDecl(idnt, rtyp, args, body));

    return func;
}


AstNode *cish::Parser::ProdFunArgs()
{
    AstNode *list = newNode(AstList());

    if (match(Type::RightParen))
        return list;

    while (true)
    {
        list->as_List.push(ProdTypeIdnt());
        if (!match(Type::Comma))
            break;
    }
    consume(Type::RightParen);

    return list;
}


AstNode *cish::Parser::ProdTypeIdnt()
{
    if (!is_typename(peek()))
    {
        consume(Type::Error, "Expected typename, received \"%s\"", m_curr->lexeme);
        return nullptr;
    }

    Token *tptok = advance();
    Token *idtok = match(Type::Identifier);

    return newNode(AstVarDecl(tptok, idtok));
}






AstNode *cish::Parser::ProdReturn()
{
    Token *kwtok = consume(Type::KwdReturn);
    if (Token *tok = match(Type::SemiColon))
        return newNode(AstReturn(newNode(AstNumber())));
    return newNode(AstReturn(ProdExprStmnt()));
}




AstNode *cish::Parser::ProdCond()
{
    if (Token *kwd = match(Type::KwdIf, Type::KwdWhile))
    {
        consume(Type::LeftParen);
        AstNode *cond = ProdExpr();
    
        AstNode *ifBody   = ProdScope();
        AstNode *elseBody = newNode(AstList()); // match(Type::KwdElse) ?  : newNode(AstList());

        return newNode(AstCond(kwd, cond, ifBody, elseBody));
    }

    consume(Type::Error, "Should be unreachable!");
    return nullptr;
}


AstNode *cish::Parser::ProdScope()
{
    auto *group = newNode(AstList());
    consume(Type::LeftBrace);
    while (!isAtEnd() && !match(Type::RightBrace))
        group->as_List.push(ProdStmnt());
    return group;
}


AstNode *cish::Parser::ProdExprStmnt()
{
    AstNode *expr = ProdExpr();
    consume(Type::SemiColon, "Expected \";\" after <statement>");
    return expr;
}


AstNode *cish::Parser::ProdExpr()
{
    AstNode *expr = ProdPrecedence();
    return expr;
}



AstNode *cish::Parser::ProdList()
{
    if (match(Type::RightParen))
        return nullptr;

    AstNode *list = newNode(AstList());

    while (true)
    {
        list->as_List.push(ProdExpr());
        if (!match(Type::Comma))
            break;
    }
    consume(Type::RightParen);

    return list;
}



AstNode *cish::Parser::ProdPrimary()
{
    if (Token *tok = match(Type::Identifier))
    {
        if (match(Type::LeftParen))
            return newNode(AstCall(tok, ProdList()));
        return newNode(AstVar(tok));
    }

    if (Token *tok = match(Type::String))
        return newNode(AstString(tok));

    if (Token *tok = match(Type::Number))
        return newNode(AstNumber(tok));

    if (Token *tok = match(Type::LeftParen))
    {
        AstNode *expr = ProdExpr();
        consume(Type::RightParen);
        return expr;
    }

    consume(Type::Error, "Should be unreachable!");
    return nullptr;
}





















cish::Token *cish::Parser::expect( uint32_t type, const char *fmt, ... )
{
    if (m_curr->type == type)
        return m_curr;

    va_list vlist;
    va_start(vlist, fmt);
    fprintf(stderr, "[PARSE ERROR] line %u, col %u\n", m_curr->lineno, m_curr->colno);
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);

    return nullptr;
}


cish::Token *cish::Parser::consume( uint32_t type, const char *fmt, ... )
{
    if (m_curr->type == type)
        return advance();

    va_list vlist;
    va_start(vlist, fmt);
    fprintf(stderr, "[PARSE ERROR] line %u, col %u\n", m_curr->lineno, m_curr->colno);
    vfprintf(stderr, fmt, vlist);
    fprintf(stderr, "\n");
    va_end(vlist);
    assert(false);

    return nullptr;
}

cish::Token *cish::Parser::consume( uint32_t type )
{
    return consume(
        type, "Expected \"%s\", received \"%s\"",
        TypeToStr(type), m_prev->lexeme
    );
}


cish::Token *cish::Parser::advance()
{
    if (!isAtEnd())
        m_prev = m_curr++;
    return m_prev;
}


cish::Token *cish::Parser::check( uint32_t type )
{
    if (m_curr->type == type)
        return m_curr;
    return nullptr;
};

uint32_t cish::Parser::peek( int offset )
{
    return (m_curr+offset)->type;
};

bool cish::Parser::isAtEnd()
{
    return m_curr->type == Type::Eof;
};

Token *cish::Parser::save()
{
    return m_curr;
}

void cish::Parser::restore( Token *tok )
{
    // printf("[restore] [%lu, %lu] <--", (m_prev-m_beg), (m_curr-m_beg));
    if (tok > m_beg)
        m_prev = tok-1;
    m_curr = tok;
    // printf(" [%lu, %lu]\n", (m_prev-m_beg), (m_curr-m_beg));
}



