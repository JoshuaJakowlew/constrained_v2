#pragma once

#include <cstddef>
#include <type_traits>
#include <concepts>

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
    struct value_pack;
}

namespace ct::traits {
    template <typename T>
    struct is_value_pack : std::false_type {};

    template <auto... Xs>
    struct is_value_pack<ct::value_pack<Xs...>> : std::true_type {};

    template <typename T>
    concept value_pack = is_value_pack<T>::value;

    template <typename T>
    concept unwrappable = value_pack<T> && requires { T::unwrap; };

    template <auto F, auto... Xs>
    consteval bool is_mappable(ct::value_pack<Xs...>) {
        return (([] {
            if constexpr (std::invocable<decltype(F), decltype(Xs)>) 
                return !std::is_void_v<std::invoke_result_t<decltype(F), decltype(Xs)>>;
            else
                return false;
        }()) && ...);
    }

    template <typename Pack, auto F>
    concept mappable = is_mappable<F>(Pack{});

    template <typename F, auto... Xs>
    concept thenable_impl = requires {
        typename F::template type<Xs...>;
    };

    template <typename F, auto... Xs>
    consteval bool is_thenable(ct::value_pack<Xs...>) {
        return thenable_impl<F, Xs...>;
    }

    template <typename Pack, typename F>
    concept thenable = is_thenable<F>(Pack{});    
}

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

        // Type of this value_pack
        using self = value_pack<Xs...>;

        // map :: Pack [a] -> (a -> b) -> Pack [b]
        //
        // <0, 1>::map(f) == <f(1), f(2)>
        template <auto F>
            requires traits::mappable<self, F>
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
