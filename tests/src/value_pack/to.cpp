#include <concepts>

#include <constrained/value_pack/to.hpp>

namespace ct::test {
    using p1 = value_pack<0, 1, 2>;

    template <auto... Xs>
    struct another_pack {};

    static_assert(std::same_as<empty::then<to<another_pack>>, another_pack<>>);
    static_assert(std::same_as<p1::then<to<another_pack>>, another_pack<0, 1, 2>>);
} // namespace ct::test