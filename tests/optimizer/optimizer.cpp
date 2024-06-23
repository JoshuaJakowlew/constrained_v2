#include <constrained/optimizer/optimizer.hpp>

namespace ct::test {
    constexpr auto type_eq = []<class T, class U>(T const & x, U const & y) {
        return std::same_as<T, U>;
    };

    constexpr auto foo = []{ return "foo()"; };
    constexpr auto bar = []{ return "bar()"; };
    constexpr auto baz = []{ return "baz()"; };

    static_assert(std::same_as<
        value_pack<foo, bar, baz, foo>
            ::then<common_elimination_pass<type_eq, value_pack<bar, baz>>>,
        value_pack<foo, foo>
    >);
} // namespace ct::test