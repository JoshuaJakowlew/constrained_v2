#include <concepts>

#include <constrained/value_pack/fold.hpp>

namespace ct::test::fold_ {  
    constexpr auto add = [](auto x, auto y)
    {
        return x + y;
    };
    
    consteval void test_fold_impl() {
        static_assert(detail::fold_impl<add, 1>::value == 1);
        static_assert(detail::fold_impl<add, 1, 1, 1, 1>::value == 4);
    }
    
    consteval void test_fold() {
        static_assert(std::same_as<empty::then<fold<add>>, empty>);
        static_assert(std::same_as<value_pack<1>::then<fold<add>>, value_pack<1>>);
        static_assert(std::same_as<value_pack<1, 2>::then<fold<add>>, value_pack<3>>);
        static_assert(std::same_as<value_pack<1, 2, 3, 4>::then<fold<add>>, value_pack<10>>);
    }
} // namespace ct::test::fold_