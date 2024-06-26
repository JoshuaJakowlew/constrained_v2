#pragma once

#include <constrained/value_pack/value_pack.hpp>

#include <type_traits>

#include <constrained/value_pack/contains.hpp>
#include <constrained/value_pack/concat.hpp>

namespace ct::detail {
    template <auto Eq, auto... Xs>
    struct diff_impl {};

    template <auto Eq, auto Head, auto... Tail>
    struct diff_impl<Eq, Head, Tail...>
    {
    private:
        template <auto... Ys>
        using head = std::conditional_t<
            value_pack<Ys...>
                ::template then<contains<Head, Eq>>
                ::unwrap,
            empty,
            value_pack<Head>    
        >;

        template <auto... Ys>
        using tail = diff_impl<Eq, Tail...>
            ::template type<Ys...>;

    public:
        template <auto... Ys>
        using type = head<Ys...>::template then<concat_pack<tail<Ys...>>>;
    };

    template <auto Eq, auto Head>
    struct diff_impl<Eq, Head>
    {
        template <auto... Ys>
        using type = std::conditional_t<
            value_pack<Ys...>
                ::template then<contains<Head, Eq>>
                ::unwrap,
            empty,
            value_pack<Head>    
        >;
    };

    template <auto Eq>
    struct diff_impl<Eq>
    {
        template <auto... Ys>
        using type = empty;
    };
} // namespace ct::detail

namespace ct {
    template <auto Eq, auto... Ys>
    struct diff
    {
        template <auto... Xs>
        using type = detail::diff_impl<Eq, Xs...>::template type<Ys...>;
    };

    template <auto... Ys>
    using diff_by_eq = diff<std::equal_to<>{}, Ys...>;
} // namespace ct