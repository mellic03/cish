#pragma once
// #include <cish/parser.hpp>
#include <cish/token-stream.hpp>
#include <cish/parse-node.hpp>
#include <cish/prod-node.hpp>

namespace parsegen
{
    using ProdNode  = cish::ProdNode;
    using ParseNode = cish::ParseNode;

    template <typename FunA, typename... Rest>
    struct OR
    {
        FunA        funA;
        OR<Rest...> funB;

        ProdNode *operator()( cish::TokenStream &P )
        {
            auto *state = P.save();
            if (auto *A = funA(P)) return A;
            if (auto *B = funB(P)) return B;
            P.restore(state);
            return nullptr;
        }
    };

    template <typename FunA>
    struct OR<FunA> {
        FunA funA;
        ProdNode *operator()( cish::TokenStream &P ) { return funA(P); }
    };

    template <>
    struct OR<void> {
        ProdNode *operator()(cish::TokenStream&) { assert((false)); return nullptr; }
    };



    template <typename FunA, typename... Rest>
    struct SEQ
    {
        FunA         funA;
        SEQ<Rest...> funB;

        ProdNode *operator()( cish::TokenStream &P )
        {
            auto *state = P.save();
            if (auto *A = funA(P))
                if (auto *B = funB(P))
                    return B;
            P.restore(state);
            return nullptr;
        }
    };

    template <typename FunA>
    struct SEQ<FunA> {
        FunA funA;
        ProdNode *operator()( cish::TokenStream &P ) { return funA(P); }
    };

    template <>
    struct SEQ<void> {
        ProdNode *operator()(cish::TokenStream&) { assert((false)); return nullptr; }
    };



    template <typename FunA>
    struct Kleene
    {
        FunA funA;

        ProdNode *operator()( cish::TokenStream &P )
        {
            cish::ProdList *list = nullptr;

            while (!P.isAtEnd())
            {
                if (ProdNode *A = funA(P))
                {
                    if (!list) list = ProdNode::New<cish::ProdList>();
                    list->push(A);
                }
    
                else
                {
                    break;
                }
            }

            return (ProdNode*)list;
        }
    };

    template <typename... Funcs>
    using KleeneSEQ = Kleene<SEQ<Funcs...>>;



    struct Taut
    {
        ProdNode *operator()( cish::TokenStream& )
        {
            return (ProdNode*)ProdNode::New<cish::ProdTrue>();
        }
    };

    template <typename FunA>
    using OPT = OR<FunA, Taut>;

}


