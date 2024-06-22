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

    template <auto... Ys>
    struct concat_pack<value_pack<Ys...>>
    {
        template <auto... Xs>
        using type = value_pack<Xs..., Ys...>;
    };

    template <class... Packs>
    struct concat_many;

    template <class Head, class... Tail>
    struct concat_many<Head, Tail...>
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

    template <class Single>
    struct concat_many<Single>
    {
        using type = Single;
    };

    template <>
    struct concat_many<>
    {
        using type = value_pack<>;
    };
} // namespace ct