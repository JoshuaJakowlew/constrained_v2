#pragma once

#include <optional>
#include <memory>

#include <constrained/constrained_type.hpp>

namespace ct::bench::manual {
    constexpr bool even(std::optional<int> x) {
        if (*x % 2 != 0) {
            x = std::nullopt;
        }
        return x.has_value();
    }
    
    inline int deref_ptr(std::unique_ptr<int> && ptr)
    {
        return ptr ? *ptr : -1;
    }

    // inline 
} // namespace ct::bench::manual