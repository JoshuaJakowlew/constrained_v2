#pragma once

#include <constrained/value_pack/value_pack.hpp>
#include <constrained/value_pack/fold.hpp>

#include <functional>

namespace ct { 
    template <auto Needle, auto Eq = std::equal_to<>{}>
    struct contains
    {
        template <auto... Xs>
        using type = decltype([]{
            if constexpr (sizeof...(Xs) == 0) return value_pack<false>{};
            else return typename value_pack<Xs...>
                ::template map<[](auto const & x) { return Eq(x, Needle); }>
                ::template then<fold<std::logical_or<>{}>>{};
        }());
    };
} // namespace ct