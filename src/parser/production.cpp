#include "production.hpp"
#include "parser.hpp"
#include <util/tuple.hpp>
#include <algorithm>
#include <type_traits>

using namespace cish;
using namespace cish::Prod;


template <typename T>
T *newProd( Parser &P )
{
    AllProd *prod;
    prod = (AllProd*)malloc(sizeof(AllProd));
    T *ptr = new (prod->as_Bytes) T(P);

    assert((prod->as_Base.m_tag == T::ProdTag()));
    assert((ptr->m_tag == T::ProdTag()));

    return ptr;
}

template <typename T>
T *newProd( Parser &P, const T &data )
{
    AllProd *prod;
    prod = (AllProd*)malloc(sizeof(AllProd));
    T *ptr = new (prod->as_Bytes) T(data);

    assert((prod->as_Base.m_tag == T::ProdTag()));
    assert((ptr->m_tag == T::ProdTag()));

    return ptr;
}





Program::Program( Parser &P )
{
    m_list = newProd<StmntList>(P);
}


Stmnt::Stmnt( Parser &P )
{
    if (P.peek() == Type::Identifier)
        as_Assign = newProd<Assign>(P);
    P.consume(Type::SemiColon);
}


StmntList::StmntList( Parser &P )
{
    m_stmnt = newProd<Stmnt>(P);
    if (!P.isAtEnd())
        m_next = newProd<StmntList>(P);
}


Decl::Decl( Parser &P )
{
    m_namedecl = newProd<NameDecl>(P);

    if (P.peek() == Type::LeftParen)
    {
        m_fundecl = newProd<FunDecl>(P);
    }

    else
    {
        P.consume(Type::SemiColon);
    }
}


Assign::Assign( Parser &P )
{
    m_idnt = newProd<Atom>(P);
    m_expr = newProd<Expr>(P);
}





NameDecl::NameDecl( Parser &P )
{
    m_type = newProd<Atom>(P);
    m_idnt = newProd<Atom>(P);
}


FunDecl::FunDecl( Parser &P)
:   m_list(nullptr)
{
    P.consume(Type::LeftParen);
    if (P.peek() == Type::Identifier)
        m_list = newProd<DeclList>(P);
    P.consume(Type::RightParen);
}


DeclList::DeclList( Parser &P )
// :   m_arg(NameDecl(P)),
    // m_next(nullptr)
{
    m_arg = newProd<NameDecl>(P);

    if (P.match(Type::SemiColon))
        m_next = newProd<DeclList>(P);
    else
        m_next = nullptr;
}




Call::Call( Parser &P )
{
    
}


CallList::CallList( Parser &P )
{

}



Expr::Expr( Parser &P )
{
    m_lhs = newProd<Term>(P);
    m_rhs = newProd<Expr>(P);
}


Term::Term( Parser &P )
{
    is_atom = (P.peek() == Type::Identifier);
    if (is_atom) as_Atom = newProd<Atom>(P);
    else         as_Expr = newProd<Expr>(P);
}


Atom::Atom( Parser &P )
{
    Token *tok = nullptr;
    if (tok = P.match(Type::Identifier, Type::Number, Type::String))
    {
        m_type = tok->type;
        m_str  = tok->lexeme;
    }
}

// bool Atom::likely( Parser &P )
// {
//     if (P.check(Type::Identifier)) return true;
//     if (P.check(Type::Number))     return true;
//     if (P.check(Type::String))     return true;
//     return false;
// }


// template <typename fn_type, typename... Args>
// struct OnFrameExit
// {
//     fn_type mF;

//     OnFrameExit( fn_type F, Args... args ): mF(F), m_args(args...) {  }
//     OnFrameExit( const OnFrameExit& ) = delete;
//     OnFrameExit( OnFrameExit&& ) = delete;
//     OnFrameExit &operator=( OnFrameExit& ) = delete;
//     OnFrameExit &operator=( OnFrameExit&& ) = delete;
//     ~OnFrameExit() { mF(m_args.value, m_args.as_base().value); }
// };





#include <stdio.h>
#include <concepts>


namespace parser_gen
{
    template <typename T>
    concept xProd = requires( T obj, Parser &P )
    {
        { obj(P) } -> std::same_as<AllProd*>;
    };

    static int indent = 0;
    #define IDNT { for (int i=0; i<indent; i++) printf(" "); }
    #define xLBL(Lb) static constexpr auto Label = Lb;

    #define xSTRUCT(Nm, Ctnt) \
    struct Nm { \
        static constexpr auto Label = #Nm; \
        AllProd *operator()( Parser &P ) Ctnt \
    };

    // xSTRUCT(xNull, { return (AllProd*)newProd<Dummy>(P); })




    template <typename Func, typename... Rest>
    struct FIRST
    {
        Func           mF;
        FIRST<Rest...> mK;

        AllProd *operator()( Parser &P )
        {
            auto *state = P.save();
            if (auto *A = mF(P)) return A;
            if (auto *B = mK(P)) return B;
            P.restore(state);
            return nullptr;
        }

    };

    template <typename Func>
    xSTRUCT(FIRST<Func>, { return mF(P); }; Func mF; )

    template <>
    xSTRUCT(FIRST<void>, { printf("FIRST<void>!!!\n"); assert((false)); return nullptr; }; )




    template <typename Func, typename... Rest>
    struct SEQ
    {
        static constexpr bool is_void = false;

        Func         mF;
        SEQ<Rest...> mK;

        AllProd *operator()( Parser &P )
        {
            auto *state = P.save();
            if (auto *A = mF(P))
                if (auto *B = mK(P))
                    return B;
            P.restore(state);
            return nullptr;
        }

    };

    template <typename Func>
    xSTRUCT(SEQ<Func>, { return mF(P); }; Func mF; static constexpr bool is_void = false; )

    template <>
    xSTRUCT(SEQ<void>, { printf("SEQ<void>!!!\n"); exit(1); return nullptr; }; static constexpr bool is_void = true; )




    template <typename T>
    struct Kleene
    {
        T mF;

        AllProd *operator()( Parser &P )
        {
            auto *list = newProd<>(P, List());

            while (!P.isAtEnd())
            {
                if (AllProd *prod = mF(P))
                    list->push(prod);
            }

            return (AllProd*)list;
        }
    };
}



template <uint32_t Tp>
struct xToken
{   AllProd *operator()( Parser &P )
    {
        auto *state = P.save();
        printf("Tp=%s, next=%s\n", TypeToStr(Tp), TypeToStr(P.peek()));
        if (Token *tok = P.match(Tp))
        {
            printf("\tmatched!\n");
            return (AllProd*)newProd<>(P, Atom(tok->type, tok->lexeme));
        }
        printf("\tno match!\n");
        P.restore(state);
    
        if (Token *tok = P.match(Type::Eof))
            return (AllProd*)newProd<>(P, Atom(tok->type, tok->lexeme));

        return nullptr; // return (AllProd*)newProd<ParseError>(P);
    }
};

template <template <typename, typename> class H, typename xxA, typename xxB>
using xHigherOrder = H<xxA, xxB>;


#include <iostream>
#include <tuple>

void functional_test( Parser &P )
{
    using namespace parser_gen;

    using xEOF        = xToken<Type::Eof>;
    using xComma      = xToken<Type::Comma>;
    using xBreak      = xToken<Type::SemiColon>;
    using xLeftParen  = xToken<Type::LeftParen>;
    using xRightParen = xToken<Type::RightParen>;
    using xType       = FIRST<xToken<Type::i32>, xToken<Type::i64>, xToken<Type::u32>, xToken<Type::u64>>;
    using xIdnt       = xToken<Type::Identifier>;
    using xString     = xToken<Type::String>;
    using xNumber     = xToken<Type::Number>;
    using xLiteral    = FIRST<xString, xNumber>;

    using TypeIdnt    = SEQ<xType, xIdnt>;
    using DeclList    = SEQ<xLeftParen, Kleene<TypeIdnt>, xRightParen>;
    using xAssign     = SEQ<xIdnt, xToken<Type::Equal>, xLiteral>;

    using xVarDecl    = SEQ<TypeIdnt, xBreak>;

    using xArgRep     = SEQ<xComma, TypeIdnt>;
    using ArgListC    = SEQ<TypeIdnt, Kleene<xArgRep>>;

    using xUsedList   = SEQ<xLeftParen, ArgListC, xRightParen>;
    using xEmptyList  = SEQ<xLeftParen, xRightParen>;
    using ArgList     = FIRST<xEmptyList, xUsedList>;

    using FunDecl     = SEQ<TypeIdnt, ArgList>;
    using xDecl       = FIRST<FunDecl, xVarDecl>;
    using xStmnt      = SEQ<xDecl, xBreak>;
    using xProg       = Kleene<xStmnt>;


    FunDecl parsefunc;
    AllProd *AST = parsefunc(P);

    if (AST)
        printf("Match!\n");
    else
        printf("No match!\n");

    // if (!AST)
    //     printf("base == nullptr\n");
    // else
    //     printf("result: %s\n", cish::Prod::ProdTagStr(AST->as_Base.m_tag));
}

