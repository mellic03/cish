// #include <cish/prod-node.hpp>
// #include <cish/token-stream.hpp>
// #include <cish/parser-gen.hpp>
// #include <stdio.h>

// using namespace cish;
// using namespace parsegen;


// template <uint32_t Tp>
// struct xToken
// {   ParseNode *operator()( TokenStream &stream )
//     {
//         auto *state = stream.save();

//         if (Token *tok = stream.match(Tp))
//             return (ParseNode*)ParseNode::New<Atom>(tok->lexeme);
//         stream.restore(state);
        
//         if (stream.match(Type::Eof))
//             return (ParseNode*)ParseNode::New<Atom>("EOF");
//             // return true;
//         stream.restore(state);
        
//         return nullptr;
//     }
// };


// xToken<Type::Eof>         xEOF;
// xToken<Type::Comma>       xComma;
// xToken<Type::SemiColon>   xBreak;
// xToken<Type::LeftParen>   xOpen;
// xToken<Type::RightParen>  xShut;
// xToken<Type::LeftBrace>   xOpenBrace;
// xToken<Type::RightBrace>  xShutBrace;
// xToken<Type::Identifier>  xIdnt;
// xToken<Type::String>      xString;
// xToken<Type::Number>      xNumber;

// xToken<Type::KwdReturn>   xKwdReturn;
// xToken<Type::KwdIf>       xKwdIf;
// xToken<Type::KwdWhile>    xKwdWhile;
// xToken<Type::KwdFor>      xKwdFor;


// xToken<Type::Equal>        xEqual;

// xToken<Type::Less>         xLES;
// xToken<Type::Greater>      xGTR;
// xToken<Type::LessEqual>    xLEQ;
// xToken<Type::GreaterEqual> xGEQ;
// xToken<Type::EqualEqual>   xEQEQ;
// xToken<Type::BangEqual>    xBangEQ;

// xToken<Type::Star>         xStar;
// xToken<Type::Slash>        xSlash;
// xToken<Type::Plus>         xPlus;
// xToken<Type::Minus>        xMinus;
// xToken<Type::PlusPlus>     xPlusPlus;
// xToken<Type::MinusMinus>   xMinusMinus;
// xToken<Type::Bang>         xBang;
// xToken<Type::Tilde>        xTilde;
// xToken<Type::Bar>          xBar;
// xToken<Type::Ampsnd>       xAmpsnd;
// xToken<Type::Hat>          xHat;


// static ParseNode *xExprFn( TokenStream& );
// static ParseNode *xStmntFn( TokenStream& );

// FPtr xExpr(xExprFn);
// FPtr xStmnt(xStmntFn);


// auto xIdntIdnt    = xIdnt & xIdnt;
// auto xAtom        = xIdnt|xString|xNumber;

// auto xParenExpr   = xOpen & xExpr & xShut;
// auto xBlockScope  = xOpenBrace & *xStmnt & xShutBrace;

// auto xVarDecl     = xIdntIdnt & *(xComma & xIdnt) & xBreak;

// auto xFunDeclArg  = xIdntIdnt;
// auto xFunDeclList = xFunDeclArg & *(xComma & xFunDeclArg);
// auto xFunDecl     = xIdntIdnt & (xOpen & ~xFunDeclList & xShut) & xBlockScope;

// auto xFunCallList = xExpr & *(xComma & xExpr);
// auto xFunCall     = xIdnt & (xOpen & ~xFunCallList & xShut);

// auto xPrimary     = xParenExpr|xFunCall|xAtom;

// auto xPostfix     = xPrimary & ~(xPlusPlus|xMinusMinus);
// auto xPrefix      = ~(xPlusPlus|xMinusMinus|xBang|xTilde) & xPostfix;

// auto xBinary4     = xPrefix  & *((xBar|xAmpsnd|xHat) & xPrefix);
// auto xBinary3     = xBinary4 & *((xStar|xSlash) & xBinary4);
// auto xBinary2     = xBinary3 & *((xPlus|xMinus) & xBinary3);
// auto xBinary1     = xBinary2 & *((xLES|xGTR|xLEQ|xGEQ|xEQEQ|xBangEQ) & xBinary2);
// auto xBinary      = xBinary1 & ~(xEqual & xExpr);

// static ParseNode *xExprFn( TokenStream &s )
// {
//     return xBinary(s);
// }


// auto xDecl        = xFunDecl | xVarDecl;
// auto xReturn      = xKwdReturn & xExpr & xBreak;
// auto xCond        = (xKwdIf|xKwdWhile|xKwdFor) & xParenExpr & xBlockScope;
// auto xStmntList   = *xStmnt;

// static ParseNode *xStmntFn( TokenStream &s )
// {
//     auto fA = xReturn | xCond | xDecl | (xExpr & xBreak);
//     return fA(s);
// }


// void ParseTest( CompileCtx &ctx, TokenStream &stream )
// {
//     while (!stream.isAtEnd())
//     {
//         if (ParseNode *node = xStmnt(stream))
//         {
//             printf("Match!\n");
//         }

//         else
//         {
//             printf("No match!\n");
//             break;
//         }
//     }

// }



