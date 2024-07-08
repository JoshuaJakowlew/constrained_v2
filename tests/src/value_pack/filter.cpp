#include <concepts>

#include <constrained/value_pack/filter.hpp>

namespace ct::test {
    using p1 = value_pack<0>;
    using p2 = value_pack<1>;
    using p3 = value_pack<0, 1, 1, 2, 3, 4, 5>;

    constexpr auto is_even = [](auto x)
    {
        return x % 2 == 0;
    };
    
    static_assert(std::same_as<filter_of<is_even>::type, value_pack<>>);
    static_assert(std::same_as<filter_of<is_even, 0>::type, value_pack<0>>);
    static_assert(std::same_as<filter_of<is_even, 1>::type, value_pack<>>);
    static_assert(std::same_as<filter_of<is_even, 0, 1, 1, 2, 3, 4, 5>::type, value_pack<0, 2, 4>>);

    static_assert(std::same_as<empty::then<filter<is_even>>, value_pack<>>);
    static_assert(std::same_as<p1::then<filter<is_even>>, value_pack<0>>);
    static_assert(std::same_as<p2::then<filter<is_even>>, value_pack<>>);
    static_assert(std::same_as<p3::then<filter<is_even>>, value_pack<0, 2, 4>>);
} // namespace ct::test