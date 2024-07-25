#include <constrained/optimizer/constraints/optimize.hpp>

namespace {
    constexpr auto type_eq = []<class T, class U>(T const & x, U const & y) {
        return std::same_as<T, U>;
    };

    constexpr auto foo = []{ return "foo()"; };
    constexpr auto bar = []{ return "bar()"; };
    constexpr auto baz = []{ return "baz()"; };
}

namespace ct::test {
    
    // pack optimisations

    static_assert(std::same_as<
        constraints::pack::optimize<type_eq>::type<>,
        empty
    >);
    
    static_assert(std::same_as<
        constraints::pack::optimize<type_eq>::type<foo>,
        value_pack<foo>
    >);

    static_assert(std::same_as<
        constraints::pack::optimize<type_eq>::type<foo, foo>,
        value_pack<foo>
    >);

    static_assert(std::same_as<
        constraints::pack::optimize<type_eq>::type<foo, bar, foo, foo, baz, bar>,
        value_pack<foo, bar, baz>
    >);

    // from-to creation

    static_assert(std::same_as<
        constraints::construct::optimize<type_eq, empty>::type<>,
        empty
    >);

    static_assert(std::same_as<
        constraints::construct::optimize<type_eq, empty>::type<foo>,
        value_pack<foo>
    >);

    static_assert(std::same_as<
        constraints::construct::optimize<type_eq, value_pack<foo>>::type<>,
        empty
    >);

    static_assert(std::same_as<
        constraints::construct::optimize<type_eq, value_pack<foo, bar>>::type<bar, baz>,
        value_pack<baz>
    >);

    static_assert(std::same_as<
        constraints::construct::optimize<type_eq, value_pack<foo, bar, foo, bar>>::type<bar, foo, baz, baz>,
        value_pack<baz>
    >);

} // namespace ct::test