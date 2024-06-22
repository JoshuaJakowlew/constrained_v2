#pragma once

#include <constrained/value_pack/diff.hpp>

namespace ct {
    template <typename T>
    concept combinator = requires (T t) {
        typename T::combinator_tag;
    };

    // template <auto... Xs>
    // consteval auto is_value_pack(value_pack<Xs...>*) -> bool { return true; }
    // consteval auto is_value_pack(auto*) -> bool { return false; }

    // template <auto Xs>
    // struct lift_if_not_value_pack
    // {
    //     using decayed_t = std::decay_t<decltype(Xs)>;
    //     using type = std::conditional_t<
    //         is_value_pack(static_cast<decayed_t*>(nullptr)),
    //         decayed_t,             // it's pack - leave it as is
    //         value_pack<Xs>         // it's not pack - wrap it
    //     >;
    //     // using type = decltype(lift(static_cast<std::decay_t<decltype(Xs)>*>(nullptr)));
    // };

    template <auto Eq, typename P1, typename P2>
    struct optimizer;

    template <auto Eq, auto... Xs, auto... Ys>
    struct optimizer<Eq, value_pack<Xs...>, value_pack<Ys...>>
    {
        using lhs = value_pack<Xs...>;
        using rhs = value_pack<Ys...>;
        using type = rhs::template then<diff_with_pack<Eq, lhs>>;
    };

    // template <auto Eq, auto... Xs, auto... Ys>
    // struct optimizer
    // {
    //     using lhs = value_pack<Xs...>;
    //     using rhs = value_pack<Ys...>;
    //     using type = rhs::template then<diff_with_pack<Eq, lhs>>;
    // };
} // namespace ct