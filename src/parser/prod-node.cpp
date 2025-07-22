#include <cish/prod-node.hpp>
#include <cish/token-stream.hpp>
#include <cish/parser-gen.hpp>
#include <stdio.h>

using namespace cish;
using namespace parsegen;

template <uint32_t Tp>
struct xToken
{   ProdNode *operator()( TokenStream &P )
    {
        auto *state = P.save();

        if (Token *tok = P.match(Tp))
            return (ProdNode*)ProdNode::New<Atom>(tok->type, tok->lexeme);

        P.restore(state);
    
        if (Token *tok = P.match(Type::Eof))
            return (ProdNode*)ProdNode::New<Atom>(tok->type, tok->lexeme);

        return nullptr;
    }
};


template <typename prod_type>
struct xIdentity
{
    ProdNode *operator()( TokenStream &stream )
    {
        prod_type prod;
        return prod(stream);
    }
};



// Program      ::= <StmntList>
// StmntList    ::= <Stmnt>*
// Stmnt        ::= <Decl> | <Assign>

// Decl         ::= <VarDecl> | <FunDecl>

// VarDecl      ::= idnt <VarDeclList> ";"
// VarDeclList  ::= idnt ("," idnt)*

// FunDecl      ::= idnt idnt "(" <FunDeclList>? ")" (";" | <FunBody>)
// FunDeclList  ::= <FunDeclArg> ("," idnt idnt)*
// FunDeclArg   ::= idnt idnt
// FunBody      ::= "{" <Stmnt>* "}"

// FunCall      ::= idnt "(" <FunCallList>? ")"
// FunCallList  ::= <FunCallArg> ("," idnt)*
// FunCallArg   ::= idnt

// Assign       ::= idnt "=" <Expr> ";"
// Expr         ::= <Binary1> <Expr>*
// Binary1      ::= <Binary2> (("+" | "-") <Binary2>)*
// Binary2      ::= <Prefix>  (("*" | "/") <Prefix>)*
// Prefix       ::= ("++" | "--" | "!" | "~")? <Postfix>
// Postfix      ::= ("++" | "--")? <Primary>
// Primary      ::= <Atom> | FunCall | "(" <Expr> ")"
// Atom         ::= idnt | string | number


using xEOF        = xToken<Type::Eof>;
using xComma      = xToken<Type::Comma>;
using xBreak      = xToken<Type::SemiColon>;
using xLeftParen  = xToken<Type::LeftParen>;
using xRightParen = xToken<Type::RightParen>;
using xIdnt       = xToken<Type::Identifier>;
using xString     = xToken<Type::String>;
using xNumber     = xToken<Type::Number>;

using xEqual      = xToken<Type::Equal>;
using xStar       = xToken<Type::Star>;
using xSlash      = xToken<Type::Slash>;
using xPlus       = xToken<Type::Plus>;
using xMinus      = xToken<Type::Minus>;
using xPlusPlus   = xToken<Type::PlusPlus>;
using xMinusMinus = xToken<Type::MinusMinus>;
using xBang       = xToken<Type::Bang>;
using xTilde      = xToken<Type::Tilde>;
using xIdntIdnt   = SEQ<xIdnt, xIdnt>;


using xVarDeclList = SEQ<xIdnt, KleeneSEQ<xComma, xIdnt>>;
using xVarDecl     = SEQ<xIdnt, xVarDeclList>;

using xFunDeclList = SEQ<xIdntIdnt, KleeneSEQ<xComma, xIdntIdnt>>;
// using xFunDeclA    = SEQ<xLeftParen, xRightParen>;
// using xFunDeclB    = SEQ<xLeftParen, xIdntIdnt, xRightParen>;
// using xFunDeclC    = SEQ<xLeftParen, xFunDeclList, xRightParen>;
// using xFunDecl     = SEQ<xIdntIdnt, OR<xFunDeclA, xFunDeclB, xFunDeclC>>;
using xFunDecl     = SEQ<xIdntIdnt, SEQ<xLeftParen, OPT<xFunDeclList>, xRightParen>>;

using xFunCallList = SEQ<xIdnt, KleeneSEQ<xComma, xIdnt>>;
using xFunCallA    = SEQ<xLeftParen, xRightParen>;
using xFunCallB    = SEQ<xLeftParen, xFunCallList, xRightParen>;
using xFunCall     = SEQ<xIdnt, OR<xFunCallA, xFunCallB>>;

// Primary  ::= <Atom> | FunCall | "(" <Expr> ")"
// Postfix  ::= <Primary> ("++" | "--")?
// Prefix   ::= ("++" | "--" | "!" | "~")? <Postfix>
// Binary2  ::= <Prefix>  (("*" | "/") <Prefix>)*
// Binary1  ::= <Binary2> (("+" | "-") <Binary2>)*
// Expr     ::= <Binary1> <Expr>*

using xAtom      = OR<xIdnt, xString, xNumber>;
using xParenExpr = SEQ<xLeftParen, xIdentity<struct xExpr>, xRightParen>;
using xPrimary   = OR<xParenExpr, xFunCall, xAtom>;

using xBinaryOp  = OR<xPlus, xMinus, xStar, xSlash>;
using xBinaryRep = KleeneSEQ<xBinaryOp, xPrimary>;
using xBinaryA   = SEQ<xPrimary, xBinaryRep>;
using xBinaryB   = xPrimary;
using xBinary    = OR<xBinaryA, xBinaryB>;

using  xExprA = SEQ<xBinary, xIdentity<struct xExpr>>;
using  xExprB = xBinary;
struct xExpr: public OR<xExprA, xExprB> {};

using xDecl      = OR<xFunDecl, xVarDecl>;
using xAssign    = SEQ<xIdnt, xEqual, xExpr>;

using xStmnt     = SEQ<OR<xDecl, xAssign>, xBreak>;
using xStmntList = Kleene<xStmnt>;



void ParseTest( TokenStream &stream )
{
    SEQ<xFunDecl, xBreak> func;
    ProdNode *node = func(stream);

    if (!node)
        printf("No match!\n");
    else
        printf("Match!\n");
}



cish::Program::Program( TokenStream &stream )
{

}


cish::StmntList::StmntList( TokenStream &stream )
{

}


cish::Stmnt::Stmnt( TokenStream &stream )
{

}


cish::Decl::Decl( TokenStream &stream )
{

}


cish::VarDecl::VarDecl( TokenStream &stream )
{
    stream.match(Type::Identifier);
}


cish::FunDecl::FunDecl( TokenStream &stream )
{

}


cish::FunDeclList::FunDeclList( TokenStream &stream )
{

}


cish::FunDeclArg::FunDeclArg( TokenStream &stream )
{

}


cish::FunBody::FunBody( TokenStream &stream )
{

}


cish::FunCall::FunCall( TokenStream &stream )
{

}


cish::FunCallList::FunCallList( TokenStream &stream )
{

}


cish::FunCallArg::FunCallArg( TokenStream &stream )
{

}


cish::Assign::Assign( TokenStream &stream )
{

}


cish::Expr::Expr( TokenStream &stream )
{

}


cish::Binary1::Binary1( TokenStream &stream )
{

}


cish::Binary2::Binary2( TokenStream &stream )
{

}


cish::Prefix::Prefix( TokenStream &stream )
{

}


cish::Postfix::Postfix( TokenStream &stream )
{

}


cish::Primary::Primary( TokenStream &stream )
{

}


cish::Atom::Atom( TokenStream &stream )
{

}


