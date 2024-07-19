#include <constrained_type.hpp>

#include <iostream>

#include <constrained/constrained_type.hpp>

namespace ct::test {
    constexpr auto is_even = [](auto x) {
        const bool res = x % 2 == 0;
        std::cout << "[is_even(" << x << ") -> " << res << "] ";
        return res;
    };

    constexpr auto gt_3 = [](auto x) {
        const bool res = x > 3;
        std::cout << "[gt_3(" << x << ") -> " << res << "] ";
        return res;
    };

    constexpr auto lt_9 = [](auto x) {
        const bool res = x < 9;
        std::cout << "[lt_9(" << x << ") -> " << res << "] ";
        return res;
    };

    constexpr auto panic = [](auto & x) {
        x = -1;
        std::cout << "panic!";
    };

    constexpr auto valid = [](auto const & x) {
        return x >= 0;
    };

    using even_int = constrained_type<int, value_pack<is_even, is_even>, value_pack<panic>, value_pack<valid>>;
    using lt_gt_even_int = constrained_type<int, value_pack<is_even, lt_9, gt_3>, value_pack<panic>, value_pack<valid>>;

    void test_ct(auto const & x)
    {
        std::cout << "even_int:\n"
            << "\tvalid() -> " << std::boolalpha << x.valid() << std::noboolalpha << '\n'
            << "\tvalue() -> " << x.value() << '\n'
            << std::endl;
    }

    void test_coonstrained_type()
    {
        // auto x1 = even_int{42};
        // test_ct(x1);

        // auto x2 = even_int{7};
        // test_ct(x2);

        // auto x3 = lt_gt_even_int{42};
        // test_ct(x3);

        // auto x4 = lt_gt_even_int{7};
        // test_ct(x4);

        // auto x5 = lt_gt_even_int{6};
        // test_ct(x5);

        // lt_gt_even_int x6{even_int{6}};
        // test_ct(x6);

        // lt_gt_even_int x7{even_int{60}};
        // test_ct(x7);

        // lt_gt_even_int x8{even_int{7}};
        // test_ct(x8);
    }
} // namespace ct::test