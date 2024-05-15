#include <iostream>
#include <optional>
#include <vector>

#include <constrained/constrained_type.hpp>
// #include <constrained/value_pack.hpp>

using namespace ct;

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

    // constexpr auto is_even = [](auto x) { return x % 2 == 0; };
    // constexpr auto gt_3 = [](auto x) { return x > 3; };
    // constexpr auto res = std::same_as<decltype(is_even), decltype(is_even)>;

    // using t = prod2<0, 1, 2>::type; // (0, 0) (0, 1) (1, 0) (1, 1)
}

/*


*/