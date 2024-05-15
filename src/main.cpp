#include <iostream>
#include <optional>
#include <vector>

#include <constrained/constrained_type.hpp>
#include <constrained/value_pack/value_pack.hpp>
#include <constrained/value_pack/concat.hpp>
#include <constrained/value_pack/contains.hpp>
#include <constrained/value_pack/diff.hpp>

using namespace ct;

using pack = value_pack<0, 1>;
        static_assert(pack::size == 2, "value_pack: invalid size");

        constexpr auto add_one = [](auto x) { return x + 1; };
        constexpr auto add_two = [](auto x) { return x + 2; };
        constexpr auto add_three = [](auto x) { return x + 3; };
        constexpr auto add_four = [](auto x) { return x + 4; };

        using cat_pack = pack
            ::then<concat<0, 1>>
            ::then<concat_pack<value_pack<2, 3>>>
            ::map<add_one>
            ::then<contains<2>>;

        constexpr auto cmp = [](auto x, auto y) { return std::same_as<decltype(x), decltype(y)>; };

        using diff_types
            = value_pack<1, 2, 2.5, 3, 4.5>
            ::then<diff_with<cmp, 1>>;

        static_assert(std::same_as<diff_types, value_pack<2.5, 4.5>>);

void test_ct(auto & ct)
{
    std::cout << "constrained: value=" << ct.value() << " valid=" << std::boolalpha << ct.valid() << std::endl;
}

int main()
{
    using namespace ct;

    constexpr static auto is_null = [](auto const & x) { return !x; };
    constexpr auto panic = [](auto & x) { std::puts("Panic!"); };
    constexpr auto fail = [](auto & x) { std::cout << "fail with value " << x << std::endl; };
    constexpr auto valid = [](auto const & x) { return is_null(x); };

    constrained_type<int, is_null, panic> x1{42};
    constrained_type<int, is_null, panic> x2{0};
    constrained_type<int, is_null, fail, valid> x3{42};
    constrained_type<int, is_null, fail, valid> x4{0};
    
    test_ct(x1);
    test_ct(x2);
    test_ct(x3);
    test_ct(x4);

    constexpr auto is_even = [](auto x) { return x % 2 == 0; };
    constexpr auto gt_3 = [](auto x) { return x > 3; };
    constexpr auto res = std::same_as<decltype(is_even), decltype(is_even)>;
}
