#include <concepts>

#include <constrained/value_pack/contains.hpp>

namespace ct::test::contains_ {
    consteval void test_contains() {
        static_assert(!empty::then<contains<1>>::unwrap); // contains [] 1 = false
        
        static_assert(value_pack<1>::then<contains<1>>::unwrap); // contains [1] 1 = true
        static_assert(!value_pack<1>::then<contains<2>>::unwrap); // contains [1] 2 = false

        static_assert(value_pack<1, 2, 3>::then<contains<2>>::unwrap); // contains [1, 2, 3] 2 = true
        static_assert(!value_pack<1, 2, 3>::then<contains<5>>::unwrap); // contains [1, 2, 3] 5 = false
    }
    
    consteval void test_contains_custom_eq() {
        constexpr auto even_eq = [](auto x, auto y)
        {
            return (x % 2 == 0) && (y % 2 == 0);
        };

        static_assert(!empty::then<contains<1, even_eq>>::unwrap); // contains [] 1 = false
        static_assert(!empty::then<contains<2, even_eq>>::unwrap); // contains [] 2 = false
        
        static_assert(!value_pack<1>::then<contains<1, even_eq>>::unwrap); // contains [1] 1 = false
        static_assert(!value_pack<1>::then<contains<2, even_eq>>::unwrap); // contains [1] 2 = false
        static_assert(!value_pack<2>::then<contains<1, even_eq>>::unwrap); // contains [2] 2 = false
        static_assert(value_pack<2>::then<contains<2, even_eq>>::unwrap); // contains [2] 2 = true
        static_assert(value_pack<2>::then<contains<6, even_eq>>::unwrap); // contains [2] 6 = true
        static_assert(value_pack<6>::then<contains<2, even_eq>>::unwrap); // contains [6] 2 = true

        static_assert(!value_pack<1, 2, 3>::then<contains<1, even_eq>>::unwrap); // contains [1, 2, 3] 1 = false
        static_assert(value_pack<1, 2, 3>::then<contains<2, even_eq>>::unwrap); // contains [1, 2, 3] 2 = true
        static_assert(!value_pack<1, 2, 3>::then<contains<5, even_eq>>::unwrap); // contains [1, 2, 3] 5 = false
        static_assert(value_pack<1, 2, 3>::then<contains<4, even_eq>>::unwrap); // contains [1, 2, 3] 4 = true
    }
} // namespace ct::test