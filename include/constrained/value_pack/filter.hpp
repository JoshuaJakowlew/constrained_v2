#pragma once

#include <type_traits>

#include <constrained/value_pack/value_pack.hpp>
#include <constrained/value_pack/concat.hpp>

namespace ct {
    template <auto Pred, auto...>
    struct filter_of {};

    template <auto Pred, auto Head, auto... Tail>
    struct filter_of<Pred, Head, Tail...>
    {
        using type = std::conditional_t<
            Pred(Head),
            value_pack<Head>,
            empty
        >::template then<concat_pack<
            typename filter_of<Pred, Tail...>::type
        >>;
    };

    template <auto Pred, auto Head>
    struct filter_of<Pred, Head>
    {
        using type = std::conditional_t<
            Pred(Head),
            value_pack<Head>,
            empty
        >;
    };

    template <auto Pred>
    struct filter_of<Pred>
    {
        using type = empty;
    };

    template <auto Pred>
    struct filter
    {
        template <auto... Xs>
        using type = filter_of<Pred, Xs...>::type;
    };
} // namespace ct