#pragma once
// #include <cish/parser.hpp>
#include <cish/token-stream.hpp>
#include <cish/parse-node.hpp>
#include <cish/prod-node.hpp>

// namespace parsegen
// {
//     using iNode = cish::iNode;

//     template <typename, typename> struct OR;
//     template <typename, typename> struct SEQ;
//     template <typename FunA> struct KPlus;
//     template <typename FunA> struct KStar;


//     struct FPtr
//     {
//         cish::iNode *(*fA)(cish::TokenStream&);
//         FPtr( cish::iNode *(*funA)(cish::TokenStream&) ): fA(funA) {  }

//         bool operator()( cish::TokenStream &stream )
//         {
//             using namespace cish;
//             auto *state = stream.save();

//             if (auto *node = fA(stream))
//                 return node;
//             stream.restore(state);

//             return false;
//         }
//     };


//     template <typename FunA, typename FunB>
//     struct OR
//     {
//         FunA fA;
//         FunB fB;
//         OR( FunA funA, FunB funB ): fA(funA), fB(funB) {  }

//         bool operator()( cish::TokenStream &P )
//         {
//             using namespace cish;
//             auto *state = P.save();

//             if (auto *node = fA(P))
//                 return node;
//             P.restore(state);

//             if (auto *node = fB(P))
//                 return node;
//             P.restore(state);

//             return false;
//         }
//     };


//     template <typename FunA, typename FunB>
//     struct SEQ
//     {
//         FunA fA;
//         FunB fB;
//         SEQ( FunA funA, FunB funB ): fA(funA), fB(funB) {  }

//         bool operator()( cish::TokenStream &stream )
//         {
//             using namespace cish;
//             auto *state = stream.save();
    
//             if (auto *lhs = fA(stream))
//                 if (auto *rhs = fB(stream))
//                     return (iNode*)iNode::New<BinaryOp>(lhs, "???", rhs);

//             stream.restore(state);
//             return false;
//         }
//     };


//     template <typename FunA>
//     struct KPlus
//     {
//         FunA fA;
//         KPlus( FunA funA ): fA(funA) {  }

//         bool operator()( cish::TokenStream &stream )
//         {
//             using namespace cish;
//             auto *state = stream.save();
//             auto *child = fA(stream);

//             if (!child)
//             {
//                 stream.restore(state);
//                 return false;
//             }

//             auto *list = iNode::New<ParseList>();
//             list->push(child);

//             while (!stream.isAtEnd())
//             {
//                 state = stream.save();
//                 child = fA(stream);

//                 if (!child)
//                 {
//                     stream.restore(state);
//                     break;
//                 }
    
//                 list->push(child);
//             }

//             if (list->size() == 1)
//                 return (iNode*)(list->front());
//             return (iNode*)list;
//         }
//     };


//     template <typename FunA>
//     struct KStar
//     {
//         FunA fA;
//         KStar( FunA funA ): fA(funA) {  }

//         bool operator()( cish::TokenStream &stream )
//         {
//             using namespace cish;
//             auto *list = iNode::New<ParseList>();

//             while (!stream.isAtEnd())
//             {
//                 auto *state = stream.save();
//                 auto *child = fA(stream);

//                 if (!child)
//                 {
//                     stream.restore(state);
//                     break;
//                 }

//                 list->push(child);
//             }

//             if (list->size() == 1)
//                 return (iNode*)(list->front());
//             return (iNode*)list;
//         }
//     };


//     template <typename FunA>
//     struct OPT
//     {
//         FunA fA;
//         OPT( FunA funA ): fA(funA) {  }

//         bool operator()( cish::TokenStream &P )
//         {
//             using namespace cish;
//             auto *state = P.save();

//             if (auto *node = fA(P))
//                 return node;
//             P.restore(state);

//             return false; // (iNode*)iNode::New<IgnoreNode>();
//         }
//     };


//     template <typename A, typename B>
//     OR<A, B> operator|( A fA, B fB ) { return OR<A, B>(fA, fB); }

//     template <typename A, typename B>
//     SEQ<A, B> operator&( A fA, B fB ) { return SEQ<A, B>(fA, fB); }

//     template <typename A>
//     KPlus<A> operator+( A fA ) { return KPlus<A>(fA); }

//     template <typename A>
//     KStar<A> operator*( A fA ) { return KStar<A>(fA); }

//     template <typename A>
//     OPT<A> operator~( A fA ) { return OPT<A>(fA); }

// }













namespace parsegen
{
    template <typename FunA, typename... Rest>
    struct OR
    {
        // OR<FunA>    funA;
        // OR<Rest...> funB;

        bool operator()( cish::TokenStream &ts )
        {
            using namespace cish;
            OR<FunA>    funA;
            OR<Rest...> funB;

            auto *state = ts.save();
            if (cish::iNode *A = funA(ts)) return true;
            if (cish::iNode *B = funB(ts)) return true;
            ts.restore(state);
            return false;
        }
    };

    template <typename FunA>
    struct OR<FunA> {
        bool operator()( cish::TokenStream &ts )
        {
            using namespace cish;
            FunA funA;
            return funA(ts);
        }
    };

    template <>
    struct OR<void> {
        bool operator()(cish::TokenStream&) { assert((false)); return false; }
    };



    template <typename FunA, typename... Rest>
    struct SEQ
    {
        bool operator()( cish::TokenStream &ts )
        {
            using namespace cish;
            SEQ<FunA>    funA;
            SEQ<Rest...> funB;

            auto *state = ts.save();
            if (cish::iNode *A = funA(ts))
                if (cish::iNode *B = funB(ts))
                    return true;
            ts.restore(state);
            return false;
        }
    };

    template <typename FunA>
    struct SEQ<FunA> {
        bool operator()( cish::TokenStream &ts )
        {
            using namespace cish;
            FunA funA;
            return funA(ts);
        }
    };

    template <>
    struct SEQ<void> {
        bool operator()(cish::TokenStream&) { assert((false)); return false; }
    };


    // template <typename FunA>
    // struct KPlus
    // {
    //     bool operator()( cish::TokenStream &stream )
    //     {
    //         using namespace cish;
    //         FunA funA;
    //         auto *state = stream.save();
    //         if (!funA(stream)) return true; // 0
    //         if (funA(stream))  return true; // 1
    //         stream.restore(state);
    //         return false;
    //     }
    // };

    // template <typename... Funcs>
    // using KPlusOR = KPlus<OR<Funcs...>>;

    // template <typename... Funcs>
    // using KPlusSEQ = KPlus<SEQ<Funcs...>>;


    // template <typename FunA>
    // struct KStar
    // {
    //     bool operator()( cish::TokenStream &stream )
    //     {
    //         using namespace cish;
    //         FunA funA;
    //         while (!stream.isAtEnd())
    //             if (funA(stream) == false)
    //                 break;
    //         return true;
    //     }
    // };

    // template <typename... Funcs>
    // using KStarOR = KStar<OR<Funcs...>>;

    // template <typename... Funcs>
    // using KStarSEQ = KStar<SEQ<Funcs...>>;



    // struct Taut
    // {
    //     bool operator()( cish::TokenStream& )
    //     {
    //         using namespace cish;
    //         return true;
    //         // return (ProdNode*)ProdNode::New<cish::ProdTrue>();
    //     }
    // };

    // template <typename FunA>
    // using OPT = OR<FunA, Taut>;


    // template <typename A, typename B>
    // OR<A, B> operator|( const A&, const B& ) { return OR<A, B>(); }

    // template <typename A, typename B>
    // SEQ<A, B> operator&( const A&, const B& ) { return SEQ<A, B>(); }

    // template <typename T>
    // KPlus<T> operator+( const T& ) { return KPlus<T>(); }

    // template <typename T>
    // KStar<T> operator*( const T& ) { return KStar<T>(); }

    // template <typename T>
    // OPT<T> operator~( const T& ) { return OPT<T>(); }


}


