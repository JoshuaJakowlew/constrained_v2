#include <concepts>

#include <constrained/value_pack/diff.hpp>

namespace ct::test::diff_ {
    consteval void test_diff() {
        // 0\0 = 0
        static_assert(std::same_as<empty::then<diff<std::not_equal_to<>{}>>, empty>);

        // 0\[1] = 0
        static_assert(std::same_as<empty::then<diff<std::not_equal_to<>{}, 1>>, empty>);

        // 0\[1, 2, 3] = 0
        static_assert(std::same_as<empty::then<diff<std::not_equal_to<>{}, 1, 2, 3>>, empty>);

        // [1]\0 = 0
        static_assert(std::same_as<value_pack<1>::then<diff<std::not_equal_to<>{}>>, value_pack<1>>);

        // [1, 2, 3]\0 = 0
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff<std::not_equal_to<>{}>>, value_pack<1, 2, 3>>);

        // [1]\[1] = 0
        static_assert(std::same_as<value_pack<1>::then<diff<std::not_equal_to<>{}, 1>>, value_pack<1>>);

        // [1, 2, 3]\[2] = [2]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff<std::not_equal_to<>{}, 2>>, value_pack<2>>);

        // [1, 2, 3]\[1, 3] = []
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff<std::not_equal_to<>{}, 1, 3>>, empty>);

        // [1, 2, 3]\[1, 1] = [1]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff<std::not_equal_to<>{}, 1, 1>>, value_pack<1>>);

        // [1, 2, 3]\[1, 2, 3] = []
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff<std::not_equal_to<>{}, 1, 2, 3>>, empty>);

        // [1, 2, 3]\[4, 5, 6] = [1, 2, 3]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff<std::not_equal_to<>{}, 4, 6, 6>>, empty>);

        // [1, 2, 3]\[2, 2, 2] = [2]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff<std::not_equal_to<>{}, 2, 2, 2>>, value_pack<2>>);
    }

    consteval void test_diff_by_eq() {
        // 0\0 = 0
        static_assert(std::same_as<empty::then<diff_by_eq<>>, empty>);

        // 0\[1] = 0
        static_assert(std::same_as<empty::then<diff_by_eq<1>>, empty>);

        // 0\[1, 2, 3] = 0
        static_assert(std::same_as<empty::then<diff_by_eq<1, 2, 3>>, empty>);

        // [1]\0 = 0
        static_assert(std::same_as<value_pack<1>::then<diff_by_eq<>>, value_pack<1>>);

        // [1, 2, 3]\0 = 0
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff_by_eq<>>, value_pack<1, 2, 3>>);

        // [1]\[1] = 0
        static_assert(std::same_as<value_pack<1>::then<diff_by_eq<1>>, empty>);

        // [1, 2, 3]\[2] = [1, 3]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff_by_eq<2>>, value_pack<1, 3>>);

        // [1, 2, 3]\[1, 3] = [2]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff_by_eq<1, 3>>, value_pack<2>>);

        // [1, 2, 3]\[1, 2, 3] = []
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff_by_eq<1, 2, 3>>, empty>);

        // [1, 2, 3]\[4, 5, 6] = [1, 2, 3]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<diff_by_eq<4, 6, 6>>, value_pack<1, 2, 3>>);
    }
} // namespace ct::test::diff_