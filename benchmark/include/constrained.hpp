#pragma once

#include <optional>
#include <memory>

#include <constrained/constrained_type.hpp>
#include <common.hpp>

namespace ct::bench::constrained {
    constexpr auto is_even = [](std::optional<int> const & x) noexcept { return x.value() % 2 == 0;};
    constexpr auto set_null_opt = [](std::optional<int> & x)noexcept { x = std::nullopt; };
    constexpr auto has_value = [](std::optional<int> const & x) noexcept { return x.has_value(); };
    using even_int = constrained_type<std::optional<int>, value_pack<is_even>, value_pack<set_null_opt>, value_pack<has_value>>;

    constexpr bool even(even_int x) noexcept
    {
        return x.valid();
    }

    constexpr auto is_not_nullptr = [](std::unique_ptr<int> const & x) noexcept { return bool(x); };
    constexpr auto set_null = [](std::unique_ptr<int>& x) noexcept { x = nullptr; };
    using non_null_ptr = constrained_type<std::unique_ptr<int>, value_pack<is_not_nullptr>, value_pack<set_null>>;

    inline int deref_ptr(non_null_ptr && ptr) noexcept
    {
        return ptr.valid() ? *ptr : -1;
    }

    constexpr auto throw_fail = [](auto &) __attribute__((always_inline)) { throw std::logic_error{""}; };

    struct throw_fail_t
    {
        constexpr auto operator()(auto&) const {
            throw std::logic_error{""};
        }
    };

    using non_null_throwing_ptr = constrained_type<std::unique_ptr<int>, value_pack<is_not_nullptr>, value_pack<throw_fail_t{}>>;
    inline int deref_ptr_throwing(non_null_throwing_ptr && ptr) {
        return *ptr;
    }

    using parsable_int = constrained_type<std::string, value_pack<non_empty, is_short, is_numeric>, value_pack<throw_fail>>;

    inline int parse_int(parsable_int const & s) {
        return std::stoi(*s);
    }


} // namespace ct::bench::constrained