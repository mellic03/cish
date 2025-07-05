#include "parser.hpp"
#include "astnode.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

using namespace cish;





/*
    <program>   ::= <stmnt>*
    <expr>      ::= <term> ((/|*|-|+) <expr>)*
    <term>      ::= idnt | string | number | <expr>
*/



AstNode *cish::Parser::buildPT( Token *buf )
{
    m_prev = nullptr;
    m_curr = buf;
    auto *root = new AstGroup();

    while (!isAtEnd())
    {
        root->push(ProdStmnt());
    }

    AstPrint AP;
    AP.visit(root);

    return root;
}


AstNode *cish::Parser::buildAST( Token *buf )
{
    AstNode *PT = buildPT(buf);
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
    // if (check(Type::KwdLet))
    //     return ProdDecl();
    if (check(Type::KwdReturn))
        return ProdReturn();
    if (check(Type::KwdIf, Type::KwdWhile))
        return ProdCond();
    return ProdExprStmnt();
}



// AstNode *cish::Parser::ProdDecl()
// {
//     Token *kwtok = consume(Type::KwdLet);

//     if (!is_typename(peek()))
//         consume(Type::Error, "Expected typename after \"let\"");

//     Token *tptok = advance();
//     Token *idtok = consume(Type::Identifier, "Expected <identifier> after \"let %s\"", tptok->lexeme);
//     AstNode *idnt = new AstVariable(tptok, idtok);
//     AstNode *decl = new AstDecl(kwtok, idnt);

//     if (match(Type::SemiColon))
//         return decl;

//     consume(Type::Equal, "Expected \"=\" after \"let <identifier>\"");
//     AstNode *vrbl = new AstVariable(tptok, idtok);
//     AstNode *expr = new AstAssign(vrbl, ProdExprStmnt());

//     auto *group = new AstGroup(decl, expr);
//     group->m_woop = false;

//     return group;
// }


AstNode *cish::Parser::ProdReturn()
{
    Token *kwtok = consume(Type::KwdReturn);
    if (Token *tok = match(Type::SemiColon))
        return new AstReturn(new AstNumber());
    return new AstReturn(ProdExprStmnt());
}




AstNode *cish::Parser::ProdCond()
{
    if (Token *kwd = match(Type::KwdIf, Type::KwdWhile))
    {
        consume(Type::LeftParen);
        AstNode *expr = ProdExpr();
        consume(Type::RightParen);
        return new AstCond(kwd, expr, ProdScope());
    }

    consume(Type::Error, "Should be unreachable!");
    return nullptr;
}


AstNode *cish::Parser::ProdScope()
{
    auto *group = new AstGroup();
    consume(Type::LeftBrace);
    while (!isAtEnd() && !match(Type::RightBrace))
        group->push(ProdStmnt());
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
    AstNode *expr = ProdPrecedence(0);
    return expr;
}


Token *cish::Parser::ProdOperator( uint8_t p, bool &is_right )
{
    const auto minfn = [](uint8_t x, uint8_t y ) { return (x < y) ? x : y; };

    static constexpr
    void *jmp[] = {
        &&P0,  &&P1,  &&P2,  &&P3,
        &&P4,  &&P5,  &&P6,  &&P7,
        &&P8,  &&P9,  &&P10, &&P11,
        &&P12, &&P13,
        &&Px
    };

    static constexpr
    size_t njmps = sizeof(jmp)/sizeof(jmp[0]);
    size_t idx   = (p<njmps) ? p : njmps-1;
    goto *jmp[idx];

    #define OP_L(...) if (auto *tok=match(__VA_ARGS__)) { is_right=false; return tok; }
    #define OP_R(...) if (auto *tok=match(__VA_ARGS__)) { is_right=true;  return tok; }

    P0: 
    P1:  OP_L( Type::Bang, Type::Tilde )
    P2:  OP_L( Type::Star, Type::Slash )
    P3:  OP_L( Type::Plus, Type::Minus )
    P4:  OP_L( Type::Less, Type::Greater, Type::LessEqual, Type::GreaterEqual,
               Type::EqualEqual, Type::BangEqual )
    P5:  OP_L( Type::Ampsnd, Type::Hat, Type::Bar )
    P6:  OP_L( Type::AmpAmpsnd, Type::BarBar )
    P7:  OP_L( Type::Equal )
    P8:
    P9:
    P10: 
    P11: 
    P12: 
    P13: 
    Px:  return nullptr;
    #undef IF_OP
}


AstNode *cish::Parser::ProdPrecedence( uint8_t p )
{
    auto *expr = ProdPostfix();
    bool is_right = false;
    while (Token *tok = ProdOperator(p, is_right))
    {
        expr = new AstBinary(expr, tok, ProdPrecedence(p+1));
        // if (is_right)
        //     expr = new AstBinary(ProdPrecedence(p), tok, expr);
        // else
        //     expr = new AstBinary(expr, tok, ProdPrecedence(p+1));
    }
    return expr;
}


AstNode *cish::Parser::ProdPostfix()
{
    AstNode *expr = ProdPrefix();
    if (Token *op = match(Type::MinusMinus, Type::PlusPlus))
        expr = new AstPostfix(expr, op);
    return expr;
}


AstNode *cish::Parser::ProdPrefix()
{
    if (Token *op = match(Type::MinusMinus, Type::PlusPlus))
        return new AstPrefix(op, ProdPrimary());

    if (Token *op = match(Type::Bang, Type::Tilde, Type::Ampsnd, Type::Minus, Type::Plus))
        return new AstPrefix(op, ProdPrimary());

    return ProdPrimary();
}


AstNode *cish::Parser::ProdList()
{
    if (match(Type::RightParen))
        return nullptr;

    AstGroup *list = new AstGroup();

    while (true)
    {
        list->push(ProdExpr());
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
            return new AstFunctionCall(tok, ProdList());
        return new AstVariable(tok);
    }

    // if (Token *tok = match(Type::String))
    //     return new AstString(tok);

    if (Token *tok = match(Type::Number))
        return new AstNumber(tok);

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
    fprintf(stderr, "[PARSE ERROR] ");
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
    fprintf(stderr, "[PARSE ERROR] ");
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





