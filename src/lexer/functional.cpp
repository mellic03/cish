





template <typename T, typename U>
struct OR_Thing
{
    T m_L; U m_R;
    OR_Thing( const T &L, const U &R ): m_L(L), m_R(R) {  };
    auto operator()( auto x )
    {
        auto Lx = m_L(x);
        if (Lx) return Lx;
        return m_R(x);
    };
};

template <typename T, typename U>
struct AND_Thing
{
    T m_L; U m_R;
    AND_Thing( const T &L, const U &R ): m_L(L), m_R(R) {  };
    auto operator()( auto x )
    {
        auto Lx = m_L(x);
        auto Rx = R_L(x);
        if (Lx && Rx)
            return Lx;
        return 0;
        // return m_R(x);
        // return m_L(x) && m_R(x);
    };
};

template <typename T, typename U>
struct BinaryThing
{
    T m_L; U m_R;
    BinaryThing( const T &L, const U &R ): m_L(L), m_R(R) {  };
    bool operator()( auto x ) { return m_L(x) || m_R(x); };
};

template <template <typename, typename> typename H, typename T, typename U>
H<T, U> HoFunc( T A, U B ) { return H<T, U>(A, B); }

template <typename T, typename U>
OR_Thing<T, U> HoFuncOR(T a, U b) { HoFunc<OR_Thing>(a, b); }

template <typename T, typename U>
AND_Thing<T, U> HoFuncAND(T a, U b) { HoFunc<AND_Thing>(a, b); }


#include <cish/node.hpp>


template <typename nd_, uint32_t tp_>
nd_  *ThingIsThing( Token *t ) { return (t->type==tp_) ? new nd_(t) : nullptr; }

auto ThingIsIdnt    = ThingIsThing<NodeBinary, Type::Identifier>;
auto ThingIsNumber  = ThingIsThing<NodeBinary, Type::Number>;
auto ThingIsString  = ThingIsThing<NodeBinary, Type::String>;
auto ThingIsPrimary = HoFuncOR(ThingIsIdnt, HoFuncOR(ThingIsNumber, ThingIsString));

auto ThingIsPlus   = ThingIsThing<NodeBinary, Type::Plus>;
auto ThingIsMinus  = ThingIsThing<NodeBinary, Type::Minus>;
auto ThingIsPMinus = HoFuncOR(ThingIsPlus, ThingIsMinus);

auto ThingIsStar   = ThingIsThing<NodeBinary, Type::Star>;
auto ThingIsSlash  = ThingIsThing<NodeBinary, Type::Slash>;

auto ThingIsTerm = HoFuncOR(ThingIsPlus, ThingIsMinus);


auto ThingIsExpr = HoFunc<OR_Thing>(ThingIsTerm, ThingIsMinus);

struct ThingExpression;


struct ThingPrimary: Node
{
    bool m_good;

    ThingPrimary( Token *tok )
    :   Node(tok),
        m_good(true)
    {
        if (m_type == Type::LeftParen)
        {
            // Node *rhs = new ThingExpression(tok);
        }
    }
};


struct ThingUnary: Node
{
    ThingUnary( Token *tok )
    :   Node(tok)
    {
        
    }
};

struct ThingUnary: Node
{
    ThingUnary( Token *tok )
    :   Node(tok) {  }
};


struct ThingCompEq: NodeBinary
{
    ThingCompEq( Token *tok )
    :   NodeBinary(tok)
    {

    }
};


struct ThingExpression: Node
{
    ThingExpression( Token *tok )
    :   Node(tok)
    {
        Node *expr = new ThingCompEq(tok);

    }
    Node *operator()( Token *tok )
    {

        return expr;
    };
};


template <typename A, typename B, typename... Args>
auto f_add( A a, B b )
{
    return [=](Args... args){ a(args...) + b(args...); };
}


template <typename fun>
auto f_rep( fun func )
{
    return [=](int a, int b){ func(a, b); func(a, b); };
}

auto is_primary = HoFunc<OR_Thing>(ThingIsIdnt, HoFunc<OR_Thing>(ThingIsNumber, ThingIsString));
auto is_term    = HoFunc<OR_Thing>(ThingIsPlus, ThingIsMinus);
auto is_factor  = HoFunc<OR_Thing>(ThingIsPlus, ThingIsMinus);

