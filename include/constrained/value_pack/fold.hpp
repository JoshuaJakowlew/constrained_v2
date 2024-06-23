#pragma once

#include <constrained/value_pack/value_pack.hpp>

namespace ct {
    template <auto Add, auto Head, auto... Tail>
    struct fold_of
    {
        static constexpr auto value = Add(Head, fold_of<Add, Tail...>::value);
    };

    template <auto Add, auto Head>
    struct fold_of<Add, Head>
    {
        static constexpr auto value = Head;
    };

    template <auto Add>
    struct fold
    {
        template <auto... Xs>
        using type = decltype([]{
            if constexpr (sizeof...(Xs) == 0) return empty{};
            else return value_pack<fold_of<Add, Xs...>::value>{};
        }());
    };
} // namespace ct