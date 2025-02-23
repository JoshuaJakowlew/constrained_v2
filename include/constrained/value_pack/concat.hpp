#pragma once

#include <constrained/value_pack/value_pack.hpp>

namespace ct {
    // Concats pack with raw values
    // value_pack<1>::then<concat<2>> -> value_pack<1, 2>
    template <auto... Ys>
    struct concat
    {
        template <auto... Xs>
        using type = value_pack<Xs..., Ys...>;
    };


    // Concats pack with other packs
    // value_pack<1>::then<concat<value_pack<2>>> -> value_pack<1, 2>
    template <typename... Packs>
    struct concat_pack;

    // multiple pack case
    template <traits::value_pack Head, traits::value_pack... Tail>
    struct concat_pack<Head, Tail...>
    {
        template <auto... Xs>
        using type = value_pack<Xs...>
            ::template then<concat_pack<Head>>
            ::template then<concat_pack<Tail...>>;
    };

    // single pack case, base case
    template <auto... Ys>
    struct concat_pack<value_pack<Ys...>>
    {
        template <auto... Xs>
        using type = value_pack<Xs..., Ys...>;
    };

    // no pack case, base case
    template <>
    struct concat_pack<>
    {
        template <auto... Xs>
        using type = value_pack<Xs...>;
    };
} // namespace ct

namespace ct::traits {
    template <typename... Packs>
    concept concat_packable = requires {
        typename concat_pack<Packs...>::template type<>;
    };
}