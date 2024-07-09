#pragma once

#include <constrained/optimizer/optimizer.hpp>

namespace ct::constraints::pack {
    template <auto Eq>
    struct optimize
    {
        template <auto... Predicates>
        using type = remove_duplicates_pass<Eq>::template type<Predicates...>;
    };
} // namespace ct::constraints::pack

namespace ct::constraints::construct {
    template <auto Eq, typename From>
    struct optimize
    {
        template <auto... Xs>
        using type = optimize_pass<Eq, From, value_pack<Xs...>>::type::result;
    };
} // namespace ct::constraints::construct