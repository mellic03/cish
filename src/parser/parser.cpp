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



AstNode *cish::Parser::buildPT( Token *buf )
{
    m_prev = nullptr;
    m_curr = buf;
    auto *root = newNode(AstList());

    while (!isAtEnd())
    {
        root->as_List.push(ProdStmnt());
    }

    // AstPrint AP;
    // AP.visit(root);

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
    AstNode *decl = newNode(AstVarDecl(tptok, idtok)); // AstVarDecl(kwtok, newNode(AstType(tptok))));

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

    Token   *rtyp = advance();
    Token   *idnt = consume(Type::Identifier, "Expected <identifier> after \"func %s\"", rtyp->lexeme);

    consume(Type::LeftParen);
    AstNode *args = ProdFunArgs();

    consume(Type::LeftBrace);
    AstNode *body = newNode(AstList());
    while (!match(Type::RightBrace))
        body->as_List.push(ProdStmnt());
    // consume(Type::RightBrace);

    return newNode(AstFunDecl(idnt, rtyp, args, body));
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
        AstNode *expr = ProdExpr();
        consume(Type::RightParen);
        return newNode(AstCond(kwd, expr, ProdScope()));
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

    #define OP_L(...) if (auto *tok=match(__VA_ARGS__)) { return tok; }
    #define OP_R(...) if (auto *tok=match(__VA_ARGS__)) { is_right=true;  return tok; }

    P0: 
    P1:  OP_L( Type::Equal )
    P2:  OP_L( Type::AmpAmpsnd, Type::BarBar )
    P3:  OP_L( Type::Ampsnd, Type::Hat, Type::Bar )
    P4:  OP_L( Type::Less, Type::Greater, Type::LessEqual, Type::GreaterEqual,
               Type::EqualEqual, Type::BangEqual )
    P5:  OP_L( Type::Plus, Type::Minus )
    P6:  OP_L( Type::Star, Type::Slash )
    P7:  OP_L( Type::Bang, Type::Tilde )
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
        expr = newNode(AstBinary(expr, tok, ProdPrecedence(p+1)));
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
        expr = newNode(AstPostfix(expr, op));
    return expr;
}


AstNode *cish::Parser::ProdPrefix()
{
    if (Token *op = match(Type::MinusMinus, Type::PlusPlus))
        return newNode(AstPrefix(op, ProdPrimary()));

    if (Token *op = match(Type::Bang, Type::Tilde, Type::Ampsnd, Type::Minus, Type::Plus))
        return newNode(AstPrefix(op, ProdPrimary()));

    return ProdPrimary();
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





