#pragma once

#include <constrained/optimizer/optimizer.hpp>
#include <constrained/value_pack/to.hpp>

#include <constrained/combinators/all.hpp>

namespace ct {
    template <auto Eq, auto... Xs, auto... Ys>
    struct optimize_pass<Eq, all<Xs...>, value_pack<Ys...>>
    {
        using type = optimize_pass<Eq, value_pack<Xs...>, value_pack<Ys...>>::type;
    };

    template <auto Eq, auto... Xs, auto... Ys>
    struct optimize_pass<Eq, value_pack<Xs...>, all<Ys...>>
    {
        using type = optimize_pass<Eq, value_pack<Xs...>, value_pack<Ys...>>::type;
    };

    template <auto Eq, auto... Xs, auto... Ys>
    struct optimize_pass<Eq, all<Xs...>, all<Ys...>>
    {
        using type = optimize_pass<Eq, value_pack<Xs...>, value_pack<Ys...>>::type;
    };
} // namespace ct