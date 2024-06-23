#pragma once

#include <constrained/value_pack/diff.hpp>

namespace ct {
    template <auto Eq, typename From, typename To>
    struct common_elimination_pass_impl
    {
    };

    template <auto Eq, auto... Xs, auto... Ys>
    struct common_elimination_pass_impl<Eq, value_pack<Xs...>, value_pack<Ys...>>
    {
        // To \ From
        using type = value_pack<Ys...>
            ::template then<diff<Eq, Xs...>>;
    };

    template <auto Eq, typename From>
    struct common_elimination_pass
    {
        template <auto... Xs>
        using type = common_elimination_pass_impl<Eq, From, value_pack<Xs...>>::type;
    };

    
} // namespace ct