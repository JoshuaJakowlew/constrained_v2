#pragma once

#include <constrained/value_pack/value_pack.hpp>

#include <type_traits>

#include <constrained/value_pack/contains.hpp>
#include <constrained/value_pack/concat.hpp>
#include <constrained/value_pack/filter.hpp>

namespace ct::detail {
    template <auto Eq, auto... Ys>
    struct not_in
    {
        template <auto X>
        constexpr bool operator()(value_pack<X>) const {
            return !value_pack<Ys...>
                ::template then<contains<X, Eq>>
                ::unwrap;
        }
    };

    struct lift
    {
        template <auto... Xs>
        using type = value_pack<value_pack<Xs>{}...>;
    };

    // value_pack<X1>{}, ..., value_pack<Xn> -> value_pack<X1, ..., Xn>
    struct unlift
    {
        template <auto... Xs>
            requires (traits::unwrappable<decltype(Xs)> && ...)
        using type = value_pack<decltype(Xs)::unwrap...>;
    };
} // namespace ct::detail

namespace ct {
    template <auto Eq, auto... Ys>
    struct diff
    {
        template <auto... Xs>
        using type = value_pack<Xs...>
            ::template then<detail::lift>
            ::template then<filter<detail::not_in<Eq, Ys...>{}>>
            ::template then<detail::unlift>;
    };
    
    template <auto... Ys>
    using diff_by_eq = diff<std::equal_to<>{}, Ys...>;
} // namespace ct