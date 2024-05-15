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
} // namespace ct