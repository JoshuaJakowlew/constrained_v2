#pragma once

#include <constrained/optimizers/optimizer.hpp>
#include <constrained/value_pack/to.hpp>

#include <constrained/combinators/all.hpp>

namespace ct {
    // all<p1> -> all<p1, p2, p3> = all<p2, p3> 
    template <auto Eq, auto... Xs, auto... Ys>
    struct optimizer<Eq, all<Xs...>, all<Ys...>>
    {
        using type = optimizer<Eq, value_pack<Xs...>, value_pack<Ys...>> // all<all<...>> ???
            ::type
            ::template then<to<all>>;
    };

    // all<p1> -> value_pack<p1, p2, p3> = all<p2, p3>
    template <auto Eq, auto... Xs, auto... Ys>
    struct optimizer<Eq, all<Xs...>, value_pack<Ys...>>
    {
        using type = optimizer<Eq, value_pack<Xs...>, value_pack<Ys...>>
            ::type
            ::template then<to<all>>;
    };

    // value_pack<p1> -> all<p1, p2, p3> = all<p2, p3>
    template <auto Eq, auto... Xs, auto... Ys>
    struct optimizer<Eq, value_pack<Xs...>, all<Ys...>>
    {
        using type = optimizer<Eq, value_pack<Xs...>, value_pack<Ys...>>
            ::type
            ::template then<to<all>>;
    };
} // namespace ct