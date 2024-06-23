#include <concepts>

#include <constrained/value_pack/fold.hpp>

namespace ct::test {
    using p1 = value_pack<1, 2>;
    
    constexpr auto add = [](auto x, auto y)
    {
        return x + y;
    };
    
    static_assert(fold_of<add, 1>::value == 1);
    static_assert(fold_of<add, 1, 2, 3, 4>::value == 10);

    static_assert(std::same_as<empty::then<fold<add>>, empty>);
    static_assert(std::same_as<value_pack<1>::then<fold<add>>, value_pack<1>>);
    static_assert(std::same_as<value_pack<1, 2>::then<fold<add>>, value_pack<3>>);
    static_assert(std::same_as<value_pack<1, 2, 3, 4>::then<fold<add>>, value_pack<10>>);
} // namespace ct::test