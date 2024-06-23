#include <concepts>

#include <constrained/value_pack/value_pack.hpp>

namespace ct::test {
    using pack = value_pack<0, 1, 2, 3>;
    constexpr auto add_one = [](auto x) { return x + 1; };

    template <auto... Xs>
    struct another_pack {};

    struct to_another_pack
    {
        template <auto... Xs>
        using type = another_pack<Xs...>;
    };

    struct sum
    {
        template <auto... Xs>
        static constexpr auto type = (... + Xs);
    };

    static_assert(pack::size == 4, "value_pack<...>::size is invalid");
    static_assert(empty::size == 0, "value_pack<...>::size is invalid");

    static_assert(std::same_as<pack::map<add_one>, value_pack<1, 2, 3, 4>>);
    static_assert(std::same_as<empty::map<add_one>, empty>);

    static_assert(std::same_as<pack::then<to_another_pack>, another_pack<0, 1, 2, 3>>);
    static_assert(std::same_as<empty::then<to_another_pack>, another_pack<>>);

    static_assert(is_value_pack<empty>::value);
    static_assert(is_value_pack<pack>::value);
    static_assert(!is_value_pack<another_pack<>>::value);
    static_assert(!is_value_pack<another_pack<0, 1, 2>>::value);

    static_assert(value_pack<42>::unwrap == 42);
    static_assert(value_pack<42>::map<add_one>::unwrap == 43);
} // namespace ct::test