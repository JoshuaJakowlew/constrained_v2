#pragma once

#include <constrained/value_pack/value_pack.hpp>

namespace ct {
    template <auto... Ys>
    struct concat
    {
        template <auto... Xs>
        using type = value_pack<Xs..., Ys...>;
    };

    template <class P>
    struct concat_pack;

    template <template <auto...> class P, auto... Ys>
    struct concat_pack<P<Ys...>>
    {
        template <auto... Xs>
        using type = value_pack<Xs..., Ys...>;
    };

    template <class Head, class... Tail>
    struct concat_many
    {
        using type = Head
            ::template then<concat_pack<
                typename concat_many<Tail...>::type
            >>;
    };

    template <class Lhs, class Rhs>
    struct concat_many<Lhs, Rhs>
    {
        using type = Lhs
            ::template then<concat_pack<Rhs>>;
    };
} // namespace ct