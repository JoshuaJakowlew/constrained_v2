#include <concepts>

#include <constrained/value_pack/unique.hpp>

namespace ct::test {
    static_assert(std::same_as<
        detail::unique_impl<std::equal_to<>{}>::type,
        empty
    >);

    static_assert(std::same_as<
        detail::unique_impl<std::equal_to<>{}, 1>::type,
        value_pack<1>
    >);
    
    static_assert(std::same_as<
        detail::unique_impl<std::equal_to<>{}, 1, 1>::type,
        value_pack<1>
    >);

    static_assert(std::same_as<
        detail::unique_impl<std::equal_to<>{}, 1, 2, 3>::type,
        value_pack<1, 2, 3>
    >);

    static_assert(std::same_as<
        detail::unique_impl<std::equal_to<>{}, 1, 1, 2, 1, 4, 3, 1, 3>::type,
        value_pack<1, 2, 4, 3>
    >);


    static_assert(std::same_as<
       empty::then<unique<>>,
       empty
    >);

    static_assert(std::same_as<
       value_pack<1>::then<unique<>>,
       value_pack<1>
    >);

    static_assert(std::same_as<
       value_pack<1, 1>::then<unique<>>,
       value_pack<1>
    >);

    static_assert(std::same_as<
       value_pack<1, 2, 3>::then<unique<>>,
       value_pack<1, 2, 3>
    >);

    static_assert(std::same_as<
       value_pack<1, 1, 2, 1, 4, 3, 1, 3>::then<unique<>>,
       value_pack<1, 2, 4, 3>
    >);
} // namespace ct::test