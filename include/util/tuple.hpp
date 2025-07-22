#pragma once
#include <stddef.h>
#include <type_traits>
#include <utility>


namespace nonstd
{
    template <typename T, typename U>
    struct pair
    {
        T first;
        U second;
    
        pair( const T &a, const U &b ): first(a), second(b) {  }
        pair( const T &a             ): pair(a, U())   {  }
        pair(                        ): pair(T(), U()) {  }

        pair( const pair  &rhs ): first(rhs.first), second(rhs.second) {  }
        pair(       pair &&rhs ): first(rhs.first), second(rhs.second) {  }
    };

    template <typename T, typename U, typename V>
    struct triple
    {
        T first;
        U second;
        V third;

        triple( const T &a, const U &b, const V &c ): first(a), second(b), third(c) {  }
        triple( const T &a, const U &b             ): triple(a,   b,   V()) {  }
        triple( const T &a                         ): triple(a,   U(), V()) {  }
        triple(                                    ): triple(T(), U(), V()) {  }

        triple( const triple  &rhs ): first(rhs.first), second(rhs.second), third(rhs.third) {  }
        triple(       triple &&rhs ): first(rhs.first), second(rhs.second), third(rhs.third) {  }

        triple( const pair<T, U> &ab ): triple(ab.first, ab.second) {  }
        triple( const pair<T, U> &ab, const V &c ): triple(ab.first, ab.second, c) {  }
        triple( const T &a, const pair<U, V> &bc ): triple(a, bc.first, bc.second) {  }

    };

    template <size_t Idx, typename T, typename U>
    decltype(auto) get( const pair<T, U> &p )
    {
        if      constexpr (Idx == 0) return p.first;
        else if constexpr (Idx == 1) return p.second;
        else    static_assert((0<=Idx && Idx<=1), "Index out of bounds");
    }

    template <size_t Idx, typename T, typename U, typename V>
    decltype(auto) get( const triple<T, U, V> &p )
    {
        if      constexpr (Idx == 0) return p.first;
        else if constexpr (Idx == 1) return p.second;
        else if constexpr (Idx == 2) return p.third;
        else    static_assert((0<=Idx && Idx<=2), "Index out of bounds");
    }

}


namespace nonstd
{
    template <typename... Items>
    struct Tuple;

    template <size_t Idx, typename T, typename... Items>
    struct Tuple_impl;

    template <size_t Idx, typename T>
    struct Tuple_impl<Idx, T>
    {
        using type = T;
        type m_value;
        Tuple_impl(): m_value() {  }
        Tuple_impl( const T &x ): m_value(x) {  }
    };

    template <size_t Idx, typename T, typename... Items>
    struct Tuple_impl: public Tuple_impl<Idx+1, Items...>
    {
        using type = T;
        using tail_type = Tuple_impl<Idx+1, Items...>;
        type m_value;
        Tuple_impl(): m_value(), tail_type() {  }
        Tuple_impl( const T &x ): m_value(x), tail_type() {  }
        Tuple_impl( const T &x, Items... items ): m_value(x), tail_type(items...) {  }
        tail_type &tail() { return static_cast<tail_type>(*this); }
        const tail_type &const_tail() const { return static_cast<tail_type>(*this);}
    };



    template <size_t I, typename T>
    struct Tuple_element;

    template <size_t I, size_t Idx, typename T, typename... Items>
    struct Tuple_element<I, Tuple_impl<Idx, T, Items...>>
        : Tuple_element<I, Tuple_impl<Idx+1, Items...>> {};

    template <size_t Idx, typename T, typename... Items>
    struct Tuple_element<Idx, Tuple_impl<Idx, T, Items...>> {
        using type = T;
    };



    // template <size_t Idx, typename... Items>
    // inline constexpr auto &Get( Tuple_impl<Idx, Items...> &tup )
    // {
    //     return tup.m_value;
    // }

    // template <size_t Idx, typename... Items>
    // inline constexpr const auto &Get( const Tuple_impl<Idx, Items...> &tup )
    // {
    //     return tup.m_value;
    // }

    // template <size_t Idx, typename... Items>
    // inline constexpr auto &&Get( const Tuple_impl<Idx, Items...> &&tup )
    // {
    //     return static_cast<decltype(tup.m_value)&&>(tup.m_value);
    // }


    template <size_t Idx, typename... Items>
    decltype(auto) get(Tuple_impl<Idx, Items...>& t)
    {
        return t.m_value;
    }

    template <size_t Idx, typename... Items>
    decltype(auto) get(const Tuple_impl<Idx, Items...>& t)
    {
        return t.m_value;
    }

    template <size_t Idx, typename... Items>
    decltype(auto) get(Tuple_impl<Idx, Items...>&& t)
    {
        return t.m_value;
    }



    template <typename... Items>
    struct Tuple: public Tuple_impl<0, Items...>
    {
        using inht_type = Tuple_impl<0, Items...>;
        using inht_type::inht_type;
    };

    template <typename T, typename U>
    struct Tuple<T, U>: public pair<T, U> {  };

    template <typename T, typename U, typename V>
    struct Tuple<T, U, V>: public triple<T, U, V> {  };
}


namespace std
{
    template <typename... Types>
    struct tuple_size<nonstd::Tuple<Types...>> {
        static constexpr size_t value = sizeof...(Types);
    };

    template <size_t I, typename... Types>
    struct tuple_element<I, nonstd::Tuple<Types...>> {
        using type = typename nonstd::Tuple_element<I, nonstd::Tuple<Types...>>::type;
    };
}


// template <size_t Idx, typename... Items>
// inline constexpr
// auto &Get( detail::Tuple_impl<Idx, Items...> &tup )
// {
//     return tup.value;
// }

// template <size_t Idx, typename... Items>
// inline constexpr
// const auto &Get( const detail::Tuple_impl<Idx, Items...> &tup )
// {
//     return tup.value;
// }

// template <size_t Idx, typename... Items>
// inline constexpr
// auto &&Get( detail::Tuple_impl<Idx, Items...> &&tup )
// {
//     return static_cast<decltype(tup.value)&&>(tup.value);
// }



// template <typename... Items>
// using Tuple = detail::Tuple_impl<0, Items...>;


