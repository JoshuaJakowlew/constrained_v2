#pragma once

#include <constrained/value_pack/value_pack.hpp>

#include <functional>

namespace ct::detail {
    template <auto Eq, auto Needle, auto Head, auto... Tail>
    struct contains_impl
    {
        static constexpr bool value =
            Eq(Needle, Head) ? true
                             : contains_impl<Eq, Needle, Tail...>::value;
    };

    template <auto Eq, auto Needle, auto Head>
    struct contains_impl<Eq, Needle, Head>
    {
        static constexpr bool value =
            Eq(Needle, Head) ? true
                             : false;
    };
} // namespace ct::detail

namespace ct { 
    template <auto Y, auto Eq = std::equal_to<>{}>
    struct contains
    {
        template <auto... Xs>
        using type = value_pack<detail::contains_impl<Eq, Y, Xs...>::value>;
    };
} // namespace ct