#include <cish/parse-node.hpp>
#include <cish/parser-gen.hpp>
#include <cish/parser.hpp>
#include <cish/token-stream.hpp>
#include <util/tuple.hpp>

#include <algorithm>
#include <type_traits>
#include <stdio.h>

using namespace cish;

// template <uint32_t Tp>
// struct xToken
// {   bool operator()( TokenStream &ts )
//     {
//         auto *state = ts.save();
//         if (Token *tok = ts.match(Tp))
//             return true;
//         ts.restore(state);
//         return false;
//     }
// };

// using namespace parsegen;

// using xEOF        = xToken<Type::Eof>;
// using xComma      = xToken<Type::Comma>;
// using xBreak      = xToken<Type::SemiColon>;
// using xLeftParen  = xToken<Type::LeftParen>;
// using xRightParen = xToken<Type::RightParen>;
// using xType       = OR<xToken<Type::i32>, xToken<Type::i64>, xToken<Type::u32>, xToken<Type::u64>>;
// using xIdnt       = xToken<Type::Identifier>;
// using xString     = xToken<Type::String>;
// using xNumber     = xToken<Type::Number>;
// using xLiteral    = OR<xString, xNumber>;
// using xIdntIdnt   = SEQ<xIdnt, xIdnt>;


cish::iNode *ProdAssign( TokenStream& );
cish::iNode *ProdExpr( TokenStream& );
cish::iNode *ProdExprStmnt( TokenStream& );
cish::iNode *ProdStmnt( TokenStream& );



cish::iNode *cish::Parse( TokenStream &ts )
{
    NodeList *list = (NodeList*)newNd<NodeList>();

    while (!ts.isAtEnd())
    {
        iNode *stmnt = ProdStmnt(ts);
        list->insert(stmnt);
    
        if (stmnt->IsError())
            break;
    }

    return list;
}


cish::BlockScope *ProdBlockScope( TokenStream &ts )
{
    auto *state = ts.save();

    if (ts.match(Type::LeftBrace))
    {
        NodeList *list = newNd<NodeList>();
        list->insert(ProdStmnt(ts));

        while (!ts.match(Type::RightBrace)) // && !list->back()->IsError())
        {
            list->insert(ProdStmnt(ts));
        }

        return newNd<BlockScope>(list);
    }

    ts.restore(state);
    return nullptr;
}


cish::iNode *ProdAssign( TokenStream &ts )
{
    auto *state = ts.save();

    auto [name, op] = ts.match2(Type::Identifier, Type::Equal);
    if (name && op) return newNd<Assign>(name->lexeme, ProdExprStmnt(ts));

    ts.restore(state); return nullptr;
}


cish::VarDecl *ProdTypeIdnt( TokenStream &ts )
{
    auto *state = ts.save();

    auto [type, name] = ts.match2(Type::Identifier, Type::Identifier);
    if (type && name) return newNd<VarDecl>(type->lexeme, name->lexeme);

    ts.restore(state);
    return nullptr;
}


cish::iNode *ProdVarDecl( TokenStream &ts )
{
    auto *state = ts.save();

    if (VarDecl *vardecl = ProdTypeIdnt(ts))
    {
        if (ts.match(Type::Equal))
        {
            NodeList *list = newNd<NodeList>();
            list->insert(vardecl);
            list->insert(newNd<Assign>(vardecl->m_name, ProdExprStmnt(ts)));
            return list;
        }

        if (ts.match(Type::SemiColon))
            return vardecl;
    }

    ts.restore(state);
    return nullptr;
}


cish::DeclList *ProdDeclArgs( TokenStream &ts )
{
    if (ts.matchSEQ(Type::LeftParen, Type::RightParen))
        return newNd<DeclList>();

    auto *state = ts.save();

    auto [lparen, type, name] = ts.match3(Type::LeftParen, Type::Identifier, Type::Identifier);
    if (lparen && type && name)
    {
        DeclList *list = newNd<DeclList>();
        list->insert(newNd<VarDecl>(type, name));

        while (ts.match(Type::Comma))
        {
            auto [tp, nm] = ts.match2(Type::Identifier, Type::Identifier);
            list->insert(newNd<VarDecl>(tp, nm));
        }

        ts.consume(Type::RightParen);
        return list;
    }

    ts.restore(state);
    return nullptr;
}

cish::iNode *ProdFunDecl( TokenStream &ts )
{
    auto *state = ts.save();
    
    auto [type, name] = ts.match2(Type::Identifier, Type::Identifier);
    if (type && name)
    {
        DeclList   *args = ProdDeclArgs(ts);
        BlockScope *body = ProdBlockScope(ts);
        return newNd<FunDecl>(type->lexeme, name->lexeme, args, body);
    }

    ts.restore(state);
    return nullptr;
}


cish::iNode *ProdDecl( TokenStream &ts )
{
    auto *state = ts.save();

    if (iNode *decl = ProdVarDecl(ts)) return decl;
    if (iNode *decl = ProdFunDecl(ts)) return decl;

    ts.restore(state);
    return nullptr;
}


cish::iNode *ProdReturn( TokenStream &ts )
{
    if (ts.match(Type::KwdReturn))
        return newNd<RetNode>(ProdExprStmnt(ts));
    return nullptr;
}



cish::iNode *ProdStmnt( TokenStream &ts )
{
    auto *state = ts.save();

    if (0) return nullptr;
    else if (iNode *stmnt = ProdReturn(ts))     return stmnt;
    else if (iNode *stmnt = ProdBlockScope(ts)) return stmnt;
    else if (iNode *stmnt = ProdAssign(ts))     return stmnt;
    else if (iNode *stmnt = ProdDecl(ts))       return stmnt;
    else if (iNode *stmnt = ProdExprStmnt(ts))  return stmnt;

    return newNd<ErrorNode>(state);
}



cish::NodeList *ProdCallList( TokenStream &ts )
{
    NodeList *list = newNd<NodeList>();
    if (ts.match(Type::RightParen))
        return list;

    list->insert(ProdExpr(ts));
    while (ts.match(Type::Comma))
        list->insert(ProdExpr(ts));
    ts.consume(Type::RightParen);

    return list;
}


// cish::iNode *ProdFunCall( TokenStream &ts )
// {
//     auto *state = ts.save();

//     auto [name, lparen] = ts.match2(Type::Identifier, Type::LeftParen);
//     if (name && lparen) 

//     ts.restore(state);
//     return nullptr;
// }




cish::iNode *ProdPrimary( TokenStream &ts )
{
    if (Token *tok = ts.match(Type::Identifier))
    {
        if (ts.match(Type::LeftParen))
            return newNd<FunCall>(tok->lexeme, ProdCallList(ts));
        return newNd<Idnt>(tok->lexeme);
    }

    if (Token *tok = ts.match(Type::String))     return newNd<String>(tok->lexeme);
    if (Token *tok = ts.match(Type::Number))     return newNd<Number>(tok->lexeme);

    if (Token *tok = ts.match(Type::LeftParen))
    {
        iNode *expr = ProdExpr(ts);
        ts.consume(Type::RightParen);
        return expr;
    }

    ts.consume(Type::Error, "Should be unreachable!");
    return nullptr;
}



cish::iNode *ProdUnary( TokenStream &ts )
{
    if (Token *tok = ts.match(Type::Bang, Type::Tilde))
        return newNd<PrefixOp>(tok, ProdUnary(ts));
    return ProdPrimary(ts);
}

// cish::iNode *ProdPostfix( TokenStream &ts )
// {
//     iNode *expr = ProdPrimary(ts);
//     if (Token *tok = ts.match(Type::PlusPlus, Type::MinusMinus))
//         expr = newNd<PostfixOp>(expr, tok);
//     return expr;
// }

// cish::iNode *ProdPrefix( TokenStream &ts )
// {
//     if (Token *tok = ts.match(Type::PlusPlus, Type::MinusMinus, Type::Bang, Type::Tilde))
//         return newNd<PrefixOp>(tok, ProdPostfix(ts));
//     return ProdPostfix(ts);
// }

cish::iNode *ProdBinary4( TokenStream &ts )
{
    iNode *expr = ProdUnary(ts);
    while (Token *tok = ts.match(Type::Bar, Type::Ampsnd, Type::Hat))
        expr = newNd<BinaryOp>(expr, tok, ProdUnary(ts));
    return expr;
}

cish::iNode *ProdBinary3( TokenStream &ts )
{
    iNode *expr = ProdBinary4(ts);
    while (Token *tok = ts.match(Type::Star, Type::Slash))
        expr = newNd<BinaryOp>(expr, tok, ProdBinary4(ts));
    return expr;
}

cish::iNode *ProdBinary2( TokenStream &ts )
{
    iNode *expr = ProdBinary3(ts);
    while (Token *tok = ts.match(Type::Plus, Type::Minus))
        expr = newNd<BinaryOp>(expr, tok, ProdBinary3(ts));
    return expr;
}

cish::iNode *ProdBinary1( TokenStream &ts )
{
    iNode *expr = ProdBinary2(ts);
    while (Token *tok = ts.match(Type::Less, Type::LessEqual, Type::Greater, Type::GreaterEqual))
        expr = newNd<BinaryOp>(expr, tok, ProdBinary2(ts));
    return expr;
}

cish::iNode *ProdExpr( TokenStream &ts )
{
    iNode *expr = ProdBinary1(ts);
    while (Token *tok = ts.match(Type::EqualEqual, Type::BangEqual))
        expr = newNd<BinaryOp>(expr, tok, ProdExpr(ts));
    return expr;
}


cish::iNode *ProdExprStmnt( TokenStream &ts )
{
    auto *expr = ProdExpr(ts);
    ts.consume(Type::SemiColon);
    return expr;
}

