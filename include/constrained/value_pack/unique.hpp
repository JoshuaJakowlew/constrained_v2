#pragma once

#include <constrained/value_pack/value_pack.hpp>
#include <constrained/value_pack/filter.hpp>

#include <functional>

namespace ct {

    template <auto Eq, auto X>
    struct not_equal
    {
        static constexpr auto func = [](auto const & y) {
            return !Eq(X, y);
        };
    };

    template <auto Eq, auto... Tail>
    struct unique_impl {};

    template <auto Eq>
    struct to_unique_impl
    {
        template <auto... Xs>
        using type = unique_impl<Eq, Xs...>::type;
    };  

    template <auto Eq, auto Head, auto... Tail>
    struct unique_impl<Eq, Head, Tail...>
    {
        using filtered_tail = value_pack<Tail...>
            ::template then<
                filter<not_equal<Eq, Head>::func>
            >;

        using unique_call = filtered_tail
            ::template then<to_unique_impl<Eq>>;

        using type = value_pack<Head>
            ::template then<
                concat_pack<unique_call>
            >;
    };

    template <auto Eq, auto Head>
    struct unique_impl<Eq, Head>
    {
        using type = value_pack<Head>;
    };
    
    template <auto Eq>
    struct unique_impl<Eq>
    {
        using type = empty;
    };

    template <auto Eq = std::equal_to<>{}>
    struct unique
    {
        template <auto... Xs>
        using type = unique_impl<Eq, Xs...>::type;
    };
} // namespace ct