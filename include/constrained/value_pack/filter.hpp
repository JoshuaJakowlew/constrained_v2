#pragma once

#include <type_traits>

#include <constrained/value_pack/value_pack.hpp>
#include <constrained/value_pack/concat.hpp>

namespace ct::detail {
    template <auto Pred, auto...>
    struct filter_impl {};

    template <auto Pred, auto Head, auto... Tail>
    struct filter_impl<Pred, Head, Tail...>
    {
        using type = std::conditional_t<
            Pred(Head),
            value_pack<Head>,
            empty
        >::template then<concat_pack<
            typename filter_impl<Pred, Tail...>::type
        >>;
    };

    template <auto Pred, auto Head>
    struct filter_impl<Pred, Head>
    {
        using type = std::conditional_t<
            Pred(Head),
            value_pack<Head>,
            empty
        >;
    };

    template <auto Pred>
    struct filter_impl<Pred>
    {
        using type = empty;
    };
}

namespace ct {
    template <auto Pred>
    struct filter
    {
        template <auto... Xs>
        using type = detail::filter_impl<Pred, Xs...>::type;
    };
} // namespace ct