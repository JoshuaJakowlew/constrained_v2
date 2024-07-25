#include <constrained/optimizer/validators/optimize.hpp>

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
        validators::pack::optimize<type_eq>::type<>,
        empty
    >);
    
    static_assert(std::same_as<
        validators::pack::optimize<type_eq>::type<foo>,
        value_pack<foo>
    >);

    static_assert(std::same_as<
        validators::pack::optimize<type_eq>::type<foo, foo>,
        value_pack<foo>
    >);

    static_assert(std::same_as<
        validators::pack::optimize<type_eq>::type<foo, bar, foo, foo, baz, bar>,
        value_pack<foo, bar, baz>
    >);
} // namespace ct::test