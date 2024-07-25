#pragma once

#include <optional>
#include <memory>

#include <constrained/constrained_type.hpp>

namespace ct::bench::constrained {
    constexpr auto is_even = [](std::optional<int> const & x) noexcept { return x.value() % 2 == 0;};
    constexpr auto set_null_opt = [](std::optional<int> & x)noexcept { x = std::nullopt; };
    constexpr auto has_value = [](std::optional<int> const & x) noexcept { return x.has_value(); };
    using even_int = constrained_type<std::optional<int>, value_pack<is_even>, value_pack<set_null_opt>, value_pack<has_value>>;

    constexpr bool even(even_int x)
    {
        return x.valid();
    }

    constexpr auto is_not_nullptr = [](std::unique_ptr<int> const & x) noexcept { return x != nullptr; };
    constexpr auto set_null = [](std::unique_ptr<int>& x)noexcept { x = nullptr; };
    using non_null_ptr = constrained_type<std::unique_ptr<int>, value_pack<is_not_nullptr>, value_pack<set_null>>;

    inline int deref_ptr(non_null_ptr && ptr)
    {
        return ptr.valid() ? *ptr : -1;
    }
} // namespace ct::bench::manual