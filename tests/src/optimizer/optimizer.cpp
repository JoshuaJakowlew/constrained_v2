#include <constrained/optimizer/optimizer.hpp>
#include <constrained/optimizer/all.hpp>
namespace ct::test {
    constexpr auto type_eq = []<class T, class U>(T const & x, U const & y) {
        return std::same_as<T, U>;
    };

    constexpr auto foo = []{ return "foo()"; };
    constexpr auto bar = []{ return "bar()"; };
    constexpr auto baz = []{ return "baz()"; };

    // common_elimination_pass

    static_assert(std::same_as<
        empty::then<common_elimination_pass<type_eq, empty>>,
        empty
    >);

    static_assert(std::same_as<
        empty::then<common_elimination_pass<type_eq, value_pack<foo, bar>>>,
        empty
    >);

    static_assert(std::same_as<
        value_pack<foo, bar>
            ::then<common_elimination_pass<type_eq, empty>>,
        value_pack<foo, bar>
    >);

    static_assert(std::same_as<
        value_pack<foo, bar>
            ::then<common_elimination_pass<type_eq, value_pack<baz>>>,
        value_pack<foo, bar>
    >);

    static_assert(std::same_as<
        value_pack<foo, bar, baz, foo>
            ::then<common_elimination_pass<type_eq, value_pack<bar, baz>>>,
        value_pack<foo, foo>
    >);

    // remove_duplicates_pass

    static_assert(std::same_as<
        empty::then<remove_duplicates_pass<type_eq>>,
        empty
    >);

    static_assert(std::same_as<
        value_pack<foo>
            ::then<remove_duplicates_pass<type_eq>>,
        value_pack<foo>
    >);

    static_assert(std::same_as<
        value_pack<foo, bar, baz>
            ::then<remove_duplicates_pass<type_eq>>,
        value_pack<foo, bar, baz>
    >);

    static_assert(std::same_as<
        value_pack<foo, bar, baz, foo>
            ::then<remove_duplicates_pass<type_eq>>,
        value_pack<foo, bar, baz>
    >);

    // optimize

    static_assert(std::same_as<
        empty::then<optimize<type_eq, empty>>,
        empty
    >);

    static_assert(std::same_as<
        value_pack<foo, bar, baz, foo>::then<optimize<type_eq, empty>>,
        value_pack<foo, bar, baz>
    >);

    static_assert(std::same_as<
        value_pack<foo, bar, baz, foo>::then<optimize<type_eq, value_pack<bar>>>,
        value_pack<foo, baz>
    >);

    static_assert(std::same_as<
        value_pack<foo, bar, baz, foo>::then<optimize<type_eq, value_pack<bar, bar, foo>>>,
        value_pack<baz>
    >);

    // optimize for all

    static_assert(std::same_as<
        optimize_pass<type_eq, empty, all<>>::type::result,
        empty
    >);

    static_assert(std::same_as<
        optimize_pass<type_eq, all<>, empty>::type::result,
        empty
    >);

    static_assert(std::same_as<
        optimize_pass<type_eq, all<>, all<>>::type::result,
        empty
    >);

    static_assert(std::same_as<
        optimize_pass<type_eq, all<bar>, value_pack<foo, bar, baz, foo>>::type::result,
        value_pack<foo, baz>
    >);

    static_assert(std::same_as<
        optimize_pass<type_eq, value_pack<bar>, all<foo, bar, baz, foo>>::type::result,
        value_pack<foo, baz>
    >);

    // FIXME: Recursive tree traversal
    static_assert(std::same_as<
        optimize_pass<type_eq, all<bar>, value_pack<foo, bar, baz, foo, all<bar>{}>>::type::result,
        value_pack<foo, baz, all<bar>{}>
    >);
} // namespace ct::test