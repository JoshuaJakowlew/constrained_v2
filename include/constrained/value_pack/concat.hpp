#pragma once

#include <constrained/value_pack/value_pack.hpp>

namespace ct {
    // Concat with values
    template <auto... Ys>
    struct concat
    {
        template <auto... Xs>
        using type = value_pack<Xs..., Ys...>;
    };

    // Concat with packs
    template <typename... Packs>
    struct concat_pack;

    template <typename Head, typename... Tail>
        requires (is_value_pack<Head>::value && ... && is_value_pack<Tail>::value)
    struct concat_pack<Head, Tail...>
    {
        template <auto... Xs>
        using type = value_pack<Xs...>
            ::template then<concat_pack<Head>>
            ::template then<concat_pack<Tail...>>;
    };

    template <auto... Ys>
    struct concat_pack<value_pack<Ys...>>
    {
        template <auto... Xs>
        using type = value_pack<Xs..., Ys...>;
    };

    template <>
    struct concat_pack<>
    {
        template <auto... Xs>
        using type = value_pack<Xs...>;
    };

    // Concat list of packs
    template <class... Packs>
    struct concat_pack_of_packs;

    template <class Head, class... Tail>
        requires (is_value_pack<Head>::value && ... && is_value_pack<Tail>::value)
    struct concat_pack_of_packs<Head, Tail...>
    {
        using type = Head
            ::template then<concat_pack<Tail...>>;
    };

    template <>
    struct concat_pack_of_packs<>
    {
        using type = value_pack<>;
    };
} // namespace ct