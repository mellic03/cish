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
        pair(): first(T()), second(U()) {  }
        pair(const T &a, const U &b ): first(a), second(b) {  }
        pair(const pair &rhs ): first(rhs.first), second(rhs.second) {  }
        pair(pair &&rhs ): first(rhs.first), second(rhs.second) {  }
    };
    
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
        // return std::move(t.m_value);
    }


    template <typename... Items>
    struct Tuple: public Tuple_impl<0, Items...>
    {
        using inht_type = Tuple_impl<0, Items...>;
        using inht_type::inht_type;
    };


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


