#pragma once

#include <regex>
#include <optional>
#include <algorithm>
#include <cctype>

namespace ct::bench {
    constexpr auto is_sorted = []<class T>(std::vector<T> const & vec) {
        return std::is_sorted(vec.cbegin(), vec.cend());
    };

    constexpr auto is_numeric = [](std::string const & s) {
        return std::all_of(s.cbegin(), s.cend(), [](auto c) { return std::isdigit(c); });
    };

    constexpr auto is_short = [](std::string const & s) {
        return s.length() <= 3;
    };

    constexpr auto non_empty = [](auto const & c) {
        return !c.empty();
    };

    struct matrix
    {
        int rows;
        int cols;
        std::vector<float> data;
    };
}
