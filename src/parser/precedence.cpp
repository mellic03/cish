#include "parser.hpp"
#include <cish/symtab.hpp>
using namespace cish;

static cish::Parser *P;


static AstNode *ProdPrec1()
{
    if (Token *op = P->match(Type::MinusMinus, Type::PlusPlus))
        return P->newNode(AstPrefix(op, P->ProdPrimary()));

    if (Token *op = P->match(Type::Bang, Type::Tilde, Type::Ampsnd))
        return P->newNode(AstPrefix(op, P->ProdPrimary()));

    if (Token *op = P->match(Type::Minus, Type::Plus))
        return P->newNode(AstPrefix(op, P->ProdPrimary()));

    return P->ProdPrimary();
}


static AstNode *ProdPrec2()
{
    AstNode *expr = ProdPrec1();
    if (Token *op = P->match(Type::MinusMinus, Type::PlusPlus))
        expr = P->newNode(AstPostfix(expr, op));
    return expr;
}


static AstNode *ProdPrec3()
{
    AstNode *expr = ProdPrec2();
    while (Token *op = P->match(Type::Star, Type::Slash))
        expr = P->newNode(AstBinary(expr, op, ProdPrec2()));
    return expr;
}


static AstNode *ProdPrec4()
{
    AstNode *expr = ProdPrec3();
    while (Token *op = P->match(Type::Plus, Type::Minus))
        expr = P->newNode(AstBinary(expr, op, ProdPrec3()));
    return expr;
}


static AstNode *ProdPrec5()
{
    AstNode *expr = ProdPrec4();
    if (Token *tok = P->match(Type::Equal))
        return P->newNode(AstAssign(tok, expr));
    return expr;
}


AstNode *cish::Parser::ProdPrecedence()
{
    P = this;
    return ProdPrec5();
}

