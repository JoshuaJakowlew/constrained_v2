#include <concepts>

#include <constrained/value_pack/to.hpp>

namespace ct::test::to_ {
    template <auto... Xs>
    struct another_pack {};

    consteval void test_to() {
        static_assert(std::same_as<empty::then<to<another_pack>>, another_pack<>>);
        static_assert(std::same_as<value_pack<0, 1, 2>::then<to<another_pack>>, another_pack<0, 1, 2>>);
    }    
} // namespace ct::test::to_