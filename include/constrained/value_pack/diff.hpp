#pragma once

#include <constrained/value_pack/value_pack.hpp>

#include <concepts>

#include <constrained/value_pack/contains.hpp>
#include <constrained/value_pack/concat.hpp>

namespace ct {
    template <class P1, class... Tail>
    struct concat_many
    {
        using type = P1
            ::template then<concat_pack<
                typename concat_many<Tail...>::type
            >>;
    };

    template <class P1, class P2>
    struct concat_many<P1, P2>
    {
        using type = P1
            ::template then<concat_pack<P2>>;
    };

    template <auto, class, class>
    struct diff_impl;

    template <auto Eq, auto... Xs, auto... Ys>
    struct diff_impl<Eq, value_pack<Xs...>, value_pack<Ys...>>
    {
    private:
        template <auto X>
        using empty_if_contains = std::conditional_t<
            std::same_as<
                typename contains<X, Eq>::template type<Ys...>,
                value_pack<true>
            >,
            value_pack<>,
            value_pack<X>
        >;
    public:
        using type = concat_many<empty_if_contains<Xs>...>::type;
    };

    template <auto Eq, auto... Ys>
    struct diff_with
    {
        template <auto... Xs>
        using type = diff_impl<Eq, value_pack<Xs...>, value_pack<Ys...>>::type;
    };

    template <auto... Ys>
    using diff = diff_with<std::equal_to<>{}, Ys...>;
} // namespace ct