#pragma once

#include <cstddef>
#include <type_traits>

namespace ct::detail {
    template <auto... Xs>
    struct unwrapper {};

    template <auto X>
    struct unwrapper<X>
    {
        static constexpr auto unwrap = X;
    };
} // namespace ct::detail

namespace ct {
    template <auto... Xs>
    struct value_pack final
        : detail::unwrapper<Xs...>
    {
    public:
        /*
         * Amount of values in a pack
         */
        static constexpr std::size_t size = sizeof...(Xs);

        // map :: Pack [a] -> (a -> b) -> Pack [b]
        //
        // <0, 1>::map(f) == <f(1), f(2)>
        template <auto F>
        using map = value_pack<F(Xs)...>;

        // then :: Pack [a] -> ([a] -> m b) -> m b
        //
        // <0, 1>::then(concat<2, 3>) == <0, 1, 2, 3>
        template <class F>
        using then = F::template type<Xs...>;
    };

    using empty = value_pack<>;

    template <typename T>
    struct is_value_pack : std::false_type {};

    template <auto... Xs>
    struct is_value_pack<value_pack<Xs...>> : std::true_type {};
} // namespace ct