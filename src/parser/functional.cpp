#include "production.hpp"
#include "parser.hpp"
#include <type_traits>

using namespace cish;
using namespace cish::Prod;


template <typename T>
static T *newProd( Parser &P )
{
    AllProd *prod;
    prod = (AllProd*)malloc(sizeof(AllProd));

    T *ptr = new (prod->as_Bytes) T(P);
    assert((prod->as_Header.m_tag == T::ProdTag()));
    // prod->tag = T::ProdTag();

    return ptr;
}

template <typename T>
static T *newProd( const T &data )
{
    AllProd *prod;
    prod = (AllProd*)malloc(sizeof(AllProd));

    T *ptr = new (prod->as_Bytes) T(data);
    assert((prod->as_Header.m_tag == T::ProdTag()));

    return ptr;
}

#include <concepts>

template <typename T>
concept xxProd = requires( Parser &P )
{
    { T::parse(P) } -> std::same_as<AllProd*>;
};


template <uint32_t Tp>
struct xxType
{
    uint32_t value = Tp;

    static AllProd *parse( Parser &P )
    {
        if (P.check(Tp))
            return (AllProd*)newProd<Atom>(P);
        return (AllProd*)newProd<ParseError>(P);
    }
};



#define xxSTRUCT(Nm, Ctnt) struct Nm { static AllProd *parse( Parser &P ) Ctnt };

xxSTRUCT(XnullX, { return nullptr; })


template <typename T> xxSTRUCT(XelseY_impl,       { return T::parse(P); } )
template <>           xxSTRUCT(XelseY_impl<void>, { return nullptr;     } )

template <xxProd Head, typename... Rest>
struct XelseY
{   static AllProd *parse( Parser &P )
    {
        if (auto *result = head(P))
            return result;
        return XelseY_impl<Rest...>::parse(P);
    }
};


template <typename T> xxSTRUCT(XthenX_impl,       { return T::parse(P); } )
template <>           xxSTRUCT(XthenX_impl<void>, { return nullptr;     } )

template <xxProd Head, typename... Rest>
struct XthenY
{   static AllProd *parse( Parser &P )
    {
        if (!Head::parse(P))
            return nullptr;
        return XthenX_impl<Rest...>::parse(P);
    }
};


using xAtom = xxType<Type::Number>;
using xPlus = xxType<Type::Plus>;
using xTerm = xxType<Type::SemiColon>;


struct xListA;
struct xListB;

using xExpr = XthenY<xAtom, xListA>;
struct xListA:  XelseY <xTerm, xListB> {};
struct xListB:  XthenY <xPlus, xExpr>  {};
// using xxExprB = xxHigherOrder<XelseY, xxAtom, xxPlus>;


template <template <typename, typename> class H, typename xxA, typename xxB>
using xxHigherOrder = H<xxA, xxB>;


using xxi32  = xxType<Type::i32>;
using xxi64  = xxType<Type::i64>;
using xDType = XelseY<xxi32, xxi64>;

using xKwLet = xxType<Type::KwdLet>;
using xKwFun = xxType<Type::KwdFunc>;
using xKwRet = xxType<Type::KwdReturn>;
using xKeywd = XelseY<xKwLet, xKwFun, xKwRet>;

using xVarDecl = XthenY< xKwLet,   xDType >;
using xFunDecl = XthenY< xKwFun,   xDType >;
using xDecl    = XelseY< xVarDecl, xFunDecl >;

using xStmnt   = XthenY<XelseY<xDecl, xExpr>, xTerm>;


struct xProg_impl_A;
struct xProg_impl_B;
struct xProg_impl_A: XthenY<xStmnt, xProg_impl_B> {};
struct xProg_impl_B: XelseY<xProg_impl_A, XnullX> {};

using xProg = xProg_impl_A;

/*
    example: 1 ("+" 1)* ";"

    atom  = xType<Type::One>
    plus  = xType<Type::Plus>

    xxExpr  = OR <atom, xxExprB>
    xxExprB = AND<plus, xxExpr>
*/



template <uint32_t TpA, uint32_t TpB>
using TypeOR = XelseY<xxType<TpA>, xxType<TpB>>;

using Prod_Numeric  = xxType<Type::Number>;
using Prod_String   = xxType<Type::String>;
using Prod_Idnt     = xxType<Type::Identifier>;
using Prod_Break    = xxType<Type::SemiColon>;
using Prod_OpEqual  = xxType<Type::Equal>;

using Prod_AddSub   = TypeOR<Type::Plus, Type::Minus>;
using Prod_MulDiv   = TypeOR<Type::Star, Type::Slash>;

// using Prod_BinOp    = XelseY<Prod_AddSub, Pr


using Prod_Primary = XelseY<Prod_Idnt, Prod_String, Prod_Numeric>;
using Prod_Literal = XelseY<Prod_Numeric, Prod_String>;
using Prod_Term    = XelseY<Prod_Primary, Prod_Literal>;

// using Prod_Expr = f_RecursiveOR<Prod_Primary, Prod_Literal, Prod_Term>;


static void testfn( Parser &P )
{
    auto A = xProg::parse(P);
}


// struct Prod_Expr
// {
//     static Base *parse( Parser &P )
//     {
//         Base *lhs = Prod_Term::parse(P);
//         if (lhs->IsError()) return lhs;

//         Base *rhs = Prod_Expr::parse(P);
//         if (rhs->IsError()) return rhs;

//         Expr *expr;
//         expr->m_lhs = (Term*)Prod_Term::parse(P);
//         expr->m_rhs = (Expr*)Prod_Expr::parse(P);

//         if (expr->m_lhs->IsError())

//         return expr;
//     }
// };

