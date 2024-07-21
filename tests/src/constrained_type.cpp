#include <constrained_type.hpp>

#include <iostream>
#include <memory>

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

    TEST_SUITE("constrained_type constructors") {
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

    TEST_SUITE("constrained_type value accessors") {
        struct counter
        {
            counter() = default;
            counter (const counter & c) noexcept
                : copies(c.copies + 1)
                , moves(c.moves)
                , moved_from(c.moved_from)
                , valid(c.valid)
            {}
            counter (counter && c)
                : copies(c.copies)
                , moves(c.moves + 1)
                , moved_from(c.moved_from)
                , valid(c.valid)
            {
                c.moved_from = true;
            }
            std::size_t copies{};
            std::size_t moves{};
            bool moved_from = false;
            bool valid = true;
        };

        constexpr auto is_valid_counter = [](counter const & c) { return c.valid; };
        using valid_counter = constrained_type<counter, value_pack<is_valid_counter>, value_pack<[](auto&){}>>;

        TEST_CASE("value") {
            counter valid_c;
            counter invalid_c; invalid_c.valid = false;

            valid_counter vc{valid_c};
            valid_counter inv_vc{invalid_c};

            SUBCASE("value") {
                const auto & c1 = vc.value();
                CHECK(c1.copies == 1);
                CHECK(c1.moves == 0);

                auto c2 = std::move(vc).value();
                CHECK(c2.copies == 1);
                CHECK(c2.moves == 1);

                const auto c3 = std::move(std::as_const(vc)).value();
                CHECK(c3.copies == 2);
                CHECK(c3.moves == 0);

                CHECK(vc.value().moved_from);
            }

            SUBCASE("value_or") {
                const auto c1 = inv_vc.value_or(valid_c);
                CHECK(c1.valid);
                
                auto c2 = inv_vc.value_or(std::move(valid_c));
                CHECK(c2.valid);

                const auto c3 = vc.value_or(invalid_c);
                CHECK(c3.valid);
                
                auto c4 = inv_vc.value_or(std::move(valid_c));
                CHECK(c4.valid);
            }

            SUBCASE("to_optional") {
                CHECK(vc.to_optional().has_value());
                CHECK_FALSE(inv_vc.to_optional().has_value());
            }
        }

        TEST_CASE("operators") {
            constexpr auto is_even_ptr  = [](std::unique_ptr<int> const & x) -> bool {
                return x && *x % 2 == 0;
            };

            constexpr auto is_even  = [](int x) -> bool {
                return x % 2 == 0;
            };

            using even_opt = constrained_type<std::unique_ptr<int>, value_pack<is_even_ptr>, value_pack<[](auto&){}>>;
            using even = constrained_type<int, value_pack<is_even>, value_pack<[](auto&){}>>;
            even_opt mb_num{new int{42}};
            even num{42};

            static_assert(std::same_as<decltype(num.value()), decltype(*num)>);
            static_assert(!std::same_as<decltype(mb_num.value()), decltype(*mb_num)>);

            static_assert(std::same_as<decltype(num.operator->()), const int*>);

            SUBCASE("operator*") {
                CHECK(*mb_num == 42);
                CHECK(*num == 42);
            }

            SUBCASE("operator*") {
                
                struct foo { int bar; };
                using foo_ptr = constrained_type<std::unique_ptr<foo>, value_pack<>, value_pack<>>;
                
                foo_ptr p{new foo{42}};
                CHECK(p->bar == 42);
                CHECK(mb_num.operator->() == std::move(mb_num).value().get());
            }
        }
    }
} // namespace ct::test