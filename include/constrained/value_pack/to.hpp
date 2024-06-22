#pragma once

#include <constrained/value_pack/value_pack.hpp>

namespace ct {
    template <template <auto...> class T>
    struct to
    {
        template <auto... Xs>
        using type = T<Xs...>;
    };
} // namespace ct