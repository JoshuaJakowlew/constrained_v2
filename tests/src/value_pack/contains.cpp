#include <concepts>

#include <constrained/value_pack/contains.hpp>

namespace ct::test {
    using p1 = value_pack<0>;
    using p2 = value_pack<0, 1, 1, 2>;

    constexpr auto even_eq = [](auto x, auto y)
    {
        return (x % 2 == 0) && (y % 2 == 0);
    };
    
    static_assert(std::same_as<empty::then<contains<0>>, value_pack<false>>);
    static_assert(std::same_as<p1::then<contains<0>>, value_pack<true>>);
    static_assert(std::same_as<p1::then<contains<42>>, value_pack<false>>);
    static_assert(std::same_as<p2::then<contains<1>>, value_pack<true>>);
    static_assert(std::same_as<p2::then<contains<42>>, value_pack<false>>);

    static_assert(std::same_as<empty::then<contains<0, even_eq>>, value_pack<false>>);
    static_assert(std::same_as<p1::then<contains<42, even_eq>>, value_pack<true>>);
    static_assert(std::same_as<p1::then<contains<43, even_eq>>, value_pack<false>>);
    static_assert(std::same_as<p2::then<contains<42, even_eq>>, value_pack<true>>);
    static_assert(std::same_as<p2::then<contains<43, even_eq>>, value_pack<false>>);
} // namespace ct::test