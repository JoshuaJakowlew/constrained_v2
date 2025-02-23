#pragma once

#include <constrained/value_pack/value_pack.hpp>

namespace ct::detail {
    template <auto Reduce, auto Head, auto... Tail>
    struct fold_impl
    {
        static constexpr auto value = Reduce(Head, fold_impl<Reduce, Tail...>::value);
    };

    template <auto Reduce, auto Head>
    struct fold_impl<Reduce, Head>
    {
        static constexpr auto value = Head;
    };
}

namespace ct {
    template <auto Reduce>
    struct fold
    {
        template <auto... Xs>
        using type = decltype([]{
            if constexpr (sizeof...(Xs) == 0) return empty{};
            else return value_pack<detail::fold_impl<Reduce, Xs...>::value>{};
        }());
    };
} // namespace ct