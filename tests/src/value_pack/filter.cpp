#include <concepts>

#include <constrained/value_pack/filter.hpp>

namespace ct::test::filter_ {
    consteval void test_filter() {
        constexpr auto is_even = [](auto x) {
            return x % 2 == 0;
        };

        // filter even [] = []
        static_assert(std::same_as<empty::then<filter<is_even>>, empty>);
        
        // filter even [1] = []
        static_assert(std::same_as<value_pack<1>::then<filter<is_even>>, empty>);
        // filter even [2] = [2]
        static_assert(std::same_as<value_pack<2>::then<filter<is_even>>, value_pack<2>>);

        // filter even [evens] = [evens]
        static_assert(std::same_as<value_pack<2, 4, 6, 4, 2>::then<filter<is_even>>, value_pack<2, 4, 6, 4, 2>>);
        // filter even [odds] = []
        static_assert(std::same_as<value_pack<1, 3, 5, 7, 9>::then<filter<is_even>>, empty>);
        // filter even [1, 2, 3, 4, 5] = [2, 4]
        static_assert(std::same_as<value_pack<1, 2, 3, 4, 5>::then<filter<is_even>>, value_pack<2, 4>>);
    }
} // namespace ct::test::filter_