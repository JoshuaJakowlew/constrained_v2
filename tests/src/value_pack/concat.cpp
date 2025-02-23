#include <concepts>

#include <constrained/value_pack/concat.hpp>

namespace ct::test::concat_ {
    consteval void test_concat() {
        static_assert(std::same_as<empty::then<concat<>>, empty>); // [] ++ [] == []
        
        static_assert(std::same_as<empty::then<concat<1>>, value_pack<1>>); // [] ++ [y] == [y]
        static_assert(std::same_as<value_pack<1>::then<concat<>>, value_pack<1>>); // [x] ++ [] == [x]
        static_assert(std::same_as<value_pack<1>::then<concat<2>>, value_pack<1, 2>>); // [x] ++ [y] == [x:y]

        static_assert(std::same_as<empty::then<concat<1, 2, 3>>, value_pack<1, 2, 3>>); // [] ++ [ys] == [ys]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<concat<>>, value_pack<1, 2, 3>>); // [xs] ++ [] == [xs]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<concat<4, 5, 6>>, value_pack<1, 2, 3, 4, 5, 6>>); // [xs] ++ [ys] == [xs:ys]
    }

    template <auto... Xs>
    struct another_pack {};

    consteval void test_concat_pack() {
        static_assert(!traits::concat_packable<another_pack<1>>); // concats only value_packs
        static_assert(traits::concat_packable<>); // ++ [] is ok
        static_assert(traits::concat_packable<empty>); // ++ [[]] is ok
        static_assert(traits::concat_packable<value_pack<1>>); // ++ [[1]] is ok
        static_assert(traits::concat_packable<value_pack<1>, empty, value_pack<2>, empty>); // ++ [[1], [], [2], []] is ok

        static_assert(std::same_as<empty::then<concat_pack<>>, empty>); // [] ++ [] = []
        static_assert(std::same_as<empty::then<concat_pack<empty>>, empty>); // [] ++ [[]] = []
        static_assert(std::same_as<empty::then<concat_pack<empty, empty>>, empty>); // [] ++ [[], []] = []

        static_assert(std::same_as<empty::then<concat_pack<value_pack<1>>>, value_pack<1>>); // [] ++ [[1]] = [1]
        static_assert(std::same_as<empty::then<concat_pack<value_pack<1>, value_pack<2, 3>>>, value_pack<1, 2, 3>>); // [] ++ [[1], [2, 3]] = [1, 2, 3]
        
        static_assert(std::same_as<value_pack<1>::then<concat_pack<empty>>, value_pack<1>>); // [1] ++ [[]] = [1]
        static_assert(std::same_as<value_pack<1, 2, 3>::then<concat_pack<empty>>, value_pack<1, 2, 3>>); // [1, 2, 3] ++ [[]] = [1, 2, 3]

        static_assert(std::same_as<value_pack<1, 2, 3>::then<concat_pack<value_pack<4, 5>>>, value_pack<1, 2, 3, 4, 5>>); // [1, 2, 3] ++ [[4, 5]] = [1, 2, 3, 4, 5]
        static_assert(std::same_as<value_pack<1, 2>::then<concat_pack<value_pack<3, 4>, value_pack<5>>>, value_pack<1, 2, 3, 4, 5>>); // [1, 2] ++ [[3, 4], [5]] = [1, 2, 3, 4, 5]

        static_assert(std::same_as<value_pack<1, 2>::then<concat_pack<empty, value_pack<3, 4>, empty, value_pack<5>, empty>>, value_pack<1, 2, 3, 4, 5>>); // [1, 2] ++ [[], [3, 4], [], [5], []] = [1, 2, 3, 4, 5]
    }
} // namespace ct::test::concat_