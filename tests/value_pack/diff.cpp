#include <concepts>

#include <constrained/value_pack/diff.hpp>

namespace ct::test {
    static_assert(std::same_as<
        ct::detail::diff_impl<std::equal_to<>{}>
            ::type<>,
        empty
    >);

    static_assert(std::same_as<
        ct::detail::diff_impl<std::equal_to<>{}>
            ::type<1, 2, 3>,
        empty
    >);

    static_assert(std::same_as<
        ct::detail::diff_impl<std::equal_to<>{}, 1, 2, 3>
            ::type<>,
        value_pack<1, 2, 3>
    >);

    static_assert(std::same_as<
        ct::detail::diff_impl<std::equal_to<>{}, 1, 2, 3, 4, 5, 6, 7>
            ::type<2, 5, 3, 1, 7>,
        value_pack<4, 6>
    >);

    static_assert(std::same_as<
        ct::detail::diff_impl<std::equal_to<>{}, 1, 2, 3, 4, 5, 6, 7>
            ::type<20, 50, 30, 10, 7>,
        value_pack<1, 2, 3, 4, 5, 6>
    >);

// -----------------------------------------------

    static_assert(std::same_as<
        empty::then<diff_by_eq<>>,
        empty
    >);

    static_assert(std::same_as<
        empty::then<diff_by_eq<1, 2, 3>>,
        empty
    >);

    static_assert(std::same_as<
        value_pack<1, 2, 3>::then<diff_by_eq<>>,
        value_pack<1, 2, 3>
    >);

    static_assert(std::same_as<
        value_pack<1, 2, 3, 4, 5, 6, 7>
            ::then<diff_by_eq<2, 5, 3, 1, 7>>,
        value_pack<4, 6>
    >);

    static_assert(std::same_as<
        value_pack<1, 2, 3, 4, 5, 6, 7>
            ::then<diff_by_eq<20, 50, 30, 10, 7>>,
        value_pack<1, 2, 3, 4, 5, 6>
    >);
} // namespace ct::test