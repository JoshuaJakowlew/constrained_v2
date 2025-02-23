#pragma once

#include <optional>
#include <memory>
#include <regex>
#include <cmath>

#include <constrained/constrained_type.hpp>
#include <common.hpp>

namespace ct::bench::manual {
    constexpr bool even(std::optional<int> x) noexcept 
    {
        if (*x % 2 != 0) {
            x = std::nullopt;
        }
        return x.has_value();
    }
    
    inline int deref_ptr(std::unique_ptr<int> && ptr) noexcept
    {
        return ptr ? *ptr : -1;
    }

    inline int deref_ptr_throwing(std::unique_ptr<int> && ptr) {
        if (!ptr) throw std::logic_error{""};
        return * ptr;
    }

    inline int parse_int(std::string const & s) {
        if (!(non_empty(s) && is_short(s) && is_numeric(s))) {
            throw std::logic_error{""};
        }
        return std::stoi(s);
    }
} // namespace ct::bench::manual