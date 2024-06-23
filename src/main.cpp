#include <iostream>
#include <optional>
#include <vector>

#include <constrained/constrained_type.hpp>

#include <constrained/value_pack/value_pack.hpp>
#include <constrained/value_pack/concat.hpp>
#include <constrained/value_pack/contains.hpp>
#include <constrained/value_pack/diff.hpp>
#include <constrained/value_pack/to.hpp>

#include <constrained/combinators/all.hpp>

#include <constrained/optimizers/optimizer.hpp>
#include <constrained/optimizers/all.hpp>

using namespace ct;

using pack = value_pack<0, 1>;
        static_assert(pack::size == 2, "value_pack: invalid size");

        constexpr auto add_n(int n, auto x) {
            std::cout << "add " << n << ": " << x << " -> " << (x + n) << std::endl;
            return x + n;
        }
        constexpr auto add_one = [](auto x) { return add_n(1, x); };
        constexpr auto add_two = [](auto x) { return add_n(2, x); };
        constexpr auto add_three = [](auto x) { return add_n(3, x); };
        constexpr auto add_four = [](auto x) { return add_n(4, x); };

        // using cat_pack = pack
        //     ::then<concat<0, 1>>
        //     ::then<concat_pack<value_pack<2, 3>>>
        //     ::map<add_one>
        //     ::then<contains<2>>;

        // constexpr auto type_eq = [](auto x, auto y) { return std::same_as<decltype(x), decltype(y)>; };

        using diff_types
            = value_pack<add_one, add_two>
            ::then<diff_with<type_eq, add_two, add_three>>;
        static_assert(std::same_as<diff_types, value_pack<add_one>>);

        using p1 = value_pack<add_one>;
        using p2 = all<add_one, add_two, add_three>;

        // all<Ps> -> Ys = all<Ys \ Ps>

        using opt1 = optimizer<
            type_eq,
            p1,
            p2
        >::type;
        static_assert(std::same_as<opt1, all<add_two, add_three>>);

        using opt2 = optimizer<
            type_eq,
            p1,
            all<add_one>
        >::type;
        static_assert(std::same_as<opt2, all<>>);

        using t1 = all<>;

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

    // constrained_type<int, is_null, panic> x1{42};
    // constrained_type<int, is_null, panic> x2{0};
    // constrained_type<int, is_null, fail, valid> x3{42};
    // constrained_type<int, is_null, fail, valid> x4{0};
    
    // test_ct(x1);
    // test_ct(x2);
    // test_ct(x3);
    // test_ct(x4);

    // constexpr auto is_even = [](auto x) {
    //     std::cout  << "[is_even]\n";
    //     return x % 2 == 0;
    // };
    // constexpr auto gt_3 = [](auto x) {
    //     std::cout << "[gt3]\n";
    //     return x > 3;
    // };
    // constexpr auto res = std::same_as<decltype(is_even), decltype(is_even)>;

    // std::cout << opt1{}(0) << std::endl;
    // std::cout << static_cast<int>(all<>{}(42)) << std::endl;

    using opt3 = optimizer<
            type_eq,
            value_pack<is_even{}>,
            all<gt_3{}, is_even{}>
        >::type;
        static_assert(std::same_as<opt3, all<gt_3{}>>);
    // opt3{}(43);
    // static_assert(std::same_as<decltype(all<gt_3, is_even>{}), decltype(all<gt_3, is_even>{})>);

    struct non_comb {};
    struct comb { using combinator_tag = void; };

    static_assert(!combinator<non_comb>);
    static_assert(combinator<comb>);

    // using constr1 = value_pack<is_even{}>;
    // using t1 = lift_if_not_value_pack<constr1{}>::type;
    // using t2 = lift_if_not_value_pack<is_even{}>::type;
    // static_assert(std::same_as<t1, value_pack<is_even{}>>);
    // static_assert(std::same_as<t2, value_pack<is_even{}>>);

    // static_assert(std::same_as<lifter<is_even{}>::type, std::false_type>);

    // using constr2 = decltype(is_even{});
    // using lift2 = decltype(lift(static_cast<constr2*>(nullptr)));
    // static_assert(std::same_as<lift2, std::false_type>);

    // static_assert(, );

    // constrained_type<int, is_even{}, fail, is_even{}> ct1(4);
    // constrained_type<int, gt_3{}, fail, is_even{}> ct2(ct1);

    // constrained_type<int, all<gt_3{}>{}, fail, is_even{}>::constrained_type()

    // std::cout << "ct1 valid: " << (int)ct1.valid() << std::endl;
    // std::cout << "ct2 valid: " << (int)ct2.valid() << std::endl;
}
