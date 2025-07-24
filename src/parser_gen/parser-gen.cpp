// #include <cish/parser.hpp>
// #include <cish/parse-node.hpp>

// #include <cish/parser-gen.hpp>
// #include <util/tuple.hpp>

// #include <stdio.h>
// #include <algorithm>
// #include <type_traits>

// using namespace cish;


// template <uint32_t Tp>
// struct xToken
// {   ProdNode *operator()( Parser &P )
//     {
//         auto *state = P.save();

//         if (Token *tok = P.match(Tp))
//             return (ProdNode*)newNode<>(P, Atom(tok->type, tok->lexeme));

//         P.restore(state);
    
//         if (Token *tok = P.match(Type::Eof))
//             return (ProdNode*)newNode<>(P, Atom(tok->type, tok->lexeme));

//         return nullptr;
//     }
// };


// template <template <typename, typename> class T, typename FunA, typename FunB>
// using HigherOrder = T<FunA, FunB>;



// #include <iostream>
// #include <tuple>

// void functional_test( cish::TokenStream &ts )
// {
//     using namespace parsegen;

//     using xEOF        = xToken<Type::Eof>;
//     using xComma      = xToken<Type::Comma>;
//     using xBreak      = xToken<Type::SemiColon>;
//     using xLeftParen  = xToken<Type::LeftParen>;
//     using xRightParen = xToken<Type::RightParen>;
//     using xType       = OR<xToken<Type::i32>, xToken<Type::i64>, xToken<Type::u32>, xToken<Type::u64>>;
//     using xIdnt       = xToken<Type::Identifier>;
//     using xString     = xToken<Type::String>;
//     using xNumber     = xToken<Type::Number>;
//     using xLiteral    = OR<xString, xNumber>;

//     using IdntIdnt    = SEQ<xIdnt, xIdnt>;
//     using DeclList    = SEQ<xLeftParen, KStar<IdntIdnt>, xRightParen>;
//     using xAssign     = SEQ<xIdnt, xToken<Type::Equal>, xLiteral>;

//     using VarDecl     = SEQ<xIdnt, xIdnt, xBreak>;

//     using xArgRep     = SEQ<xComma, IdntIdnt>;
//     using ArgListC    = SEQ<IdntIdnt, KStar<xArgRep>>;
//     using xUsedList   = SEQ<xLeftParen, ArgListC, xRightParen>;
//     using xEmptyList  = SEQ<xLeftParen, xRightParen>;
//     using ArgList     = OR<xEmptyList, xUsedList>;
//     using FunDecl     = SEQ<IdntIdnt, ArgList>;

//     using Decl        = OR<FunDecl, VarDecl>;
//     using xStmnt      = SEQ<Decl, xBreak>;
//     using xProg       = KStar<xStmnt>;


//     FunDecl parsefunc;
//     iNode *node = parsefunc(ts);

//     if (node)
//         printf("Match!\n");
//     else
//         printf("No match!\n");

//     // if (!AST)
//     //     printf("base == nullptr\n");
//     // else
//     //     printf("result: %s\n", cish::ProdTagStr(AST->as_Base.m_tag));
// }

