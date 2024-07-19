#include <constrained_type.hpp>

#include <iostream>

#include <constrained/constrained_type.hpp>

#include <doctest/doctest.h>

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

    constexpr auto gte_0 = [](auto const & x) { return x >= 0; };
    constexpr auto gt_0 = [](auto const & x) { return x > 0; };

    TEST_SUITE("constrained_type") {
        static bool fail_called = false;
        static bool check_called = false;

        static int fail_count = 0;
        static int check_count = 0;

        constexpr auto flag_check = [](auto const & x) { check_called = true; return true; };
        constexpr auto flag_on_fail = [](auto & x) { fail_called = true; };

        constexpr auto count_check = [](auto const & x) { ++check_count; return true; };
        constexpr auto count_check2 = [](auto const & x) { ++check_count; return true; };
        constexpr auto count_on_fail = [](auto & x) { ++fail_count; };

        using positive_int = constrained_type<int, value_pack<flag_check, gt_0>, value_pack<flag_on_fail>>;
        using non_negative_int = constrained_type<int, value_pack<flag_check, gte_0>, value_pack<flag_on_fail>>;

        static_assert(sizeof(positive_int) == sizeof(int));
        static_assert(std::same_as<positive_int::optimized_fail_handlers, value_pack<flag_on_fail>>);

        constexpr auto max_size_10 = [](auto const & x) { return x.size() <= 10; };
        using short_string = constrained_type<
            std::string,
            value_pack<flag_check, count_check, max_size_10>,
            value_pack<flag_on_fail, count_on_fail>,
            value_pack<max_size_10>
        >;
        using unoptimized_short_string = constrained_type<
            std::string,
            value_pack<flag_check, count_check, count_check2, max_size_10>,
            value_pack<flag_on_fail, count_on_fail>,
            value_pack<max_size_10>
        >;

        TEST_CASE("default constructor") {
            fail_called = false;
            check_called = false;

            SUBCASE("satisfied") {
                non_negative_int x;

                CHECK(check_called);
                CHECK_FALSE(fail_called);

                CHECK(x.value() == non_negative_int::value_t{});
                CHECK(x.valid());
            }
            SUBCASE("unsatisfied") {
                positive_int x;
                
                CHECK(check_called);
                CHECK(fail_called);

                CHECK(x.value() == positive_int::value_t{});
                CHECK_FALSE(x.valid());
            }
            SUBCASE("satisfied (nocheck)") {
                non_negative_int x(nocheck{});

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called);

                CHECK(x.value() == non_negative_int::value_t{});
                CHECK(x.valid());
            }
            SUBCASE("unsatisfied (nocheck)") {
                positive_int x(nocheck{});
                
                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called);

                CHECK(x.value() == positive_int::value_t{});
                CHECK_FALSE(x.valid());
            }
        }

        TEST_CASE("forwarding constructor") {
            check_called = false;
            fail_called = false;

            SUBCASE("satisfied") {
                short_string x("012345");

                CHECK(check_called);
                CHECK_FALSE(fail_called);
                CHECK(x.value() == "012345");
                CHECK(x.valid());
            }
            SUBCASE("unsatisfied") {
                short_string x("0123456789abcdef");

                CHECK(check_called);
                CHECK(fail_called);
                CHECK(x.value() == "0123456789abcdef");
                CHECK_FALSE(x.valid());
            }
        }

        TEST_CASE("copy constructor") {
            positive_int from_valid{42};
            positive_int from_invalid{-42};
            
            check_called = false;
            fail_called = false;

            SUBCASE("satisfied") {
                positive_int to(from_valid);

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called);

                CHECK(to.value() == 42);
                CHECK(to.valid());
            }
            SUBCASE("unsatisfied") {
                positive_int to(from_invalid);

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called);

                CHECK(to.value() == -42);
                CHECK_FALSE(to.valid());
            }
        }

        TEST_CASE("move constructor") {
            constexpr auto VALID_VALUE = "012345";
            constexpr auto INVALID_VALUE = "0123456789abcdef";

            std::string moved_from = VALID_VALUE;
            std::string temp = std::move(moved_from);

            short_string from_valid{VALID_VALUE};
            short_string from_invalid{INVALID_VALUE};
            
            check_called = false;
            fail_called = false;

            SUBCASE("satisfied") {
                short_string to(std::move(from_valid));

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called);
                
                CHECK(to.value() == VALID_VALUE);
                CHECK(to.valid());

                CHECK(from_valid.value() == moved_from);
                CHECK(from_valid.valid());
            }
            SUBCASE("unsatisfied") {
                short_string to(std::move(from_invalid));

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called);
                
                CHECK(to.value() == INVALID_VALUE);
                CHECK_FALSE(to.valid());

                CHECK(from_invalid.value() == moved_from);
                CHECK(from_invalid.valid());
            }
        }

        TEST_CASE("copy constructor (optimizing)") {
            constexpr auto VALID_VALUE = "012345";
            constexpr auto INVALID_VALUE = "0123456789abcdef";

            check_count = 0;
            fail_count = 0;

            short_string from_valid{VALID_VALUE};
            short_string from_invalid{INVALID_VALUE};
            
            CHECK(check_count == 2);
            CHECK(fail_count == 1);

            check_called = false;
            fail_called = false;

            check_count = 0;
            fail_count = 0;

            SUBCASE("satisfied") {
                unoptimized_short_string to(from_valid);

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called);
                CHECK(check_count == 1);

                CHECK(to.value() == VALID_VALUE);
                CHECK(to.valid());
            }
            SUBCASE("unsatisfied") {
                unoptimized_short_string to(from_invalid);

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called); // no need to re-check if from value is invalid as validators are the same
                CHECK(check_count == 0);
                CHECK(fail_count == 0);

                CHECK(to.value() == INVALID_VALUE);
                CHECK_FALSE(to.valid());
            }
        }

        TEST_CASE("move constructor (optimizing)") {
            constexpr auto VALID_VALUE = "012345";
            constexpr auto INVALID_VALUE = "0123456789abcdef";

            std::string moved_from = VALID_VALUE;
            std::string temp = std::move(moved_from);

            check_count = 0;
            fail_count = 0;

            short_string from_valid{VALID_VALUE};
            short_string from_invalid{INVALID_VALUE};
            
            CHECK(check_count == 2);
            CHECK(fail_count == 1);

            check_called = false;
            fail_called = false;

            check_count = 0;
            fail_count = 0;

            SUBCASE("satisfied") {
                unoptimized_short_string to(std::move(from_valid));

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called);
                CHECK(check_count == 1);
                
                CHECK(to.value() == VALID_VALUE);
                CHECK(to.valid());
            }
            SUBCASE("unsatisfied") {
                unoptimized_short_string to(std::move(from_invalid));

                CHECK_FALSE(check_called);
                CHECK_FALSE(fail_called); // no need to re-check if from value is invalid as validators are the same
                CHECK(check_count == 0);
                CHECK(fail_count == 0);
                
                CHECK(to.value() == INVALID_VALUE);
                CHECK_FALSE(to.valid());

                CHECK(from_invalid.value() == moved_from);
                CHECK(from_invalid.valid());
            }
        }
    }
} // namespace ct::test