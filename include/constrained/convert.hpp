#pragma once

#include <constrained/constrained_type.hpp>

namespace ct {
    template <typename Wrapper, auto F>
    struct wrapper : Wrapper
    {
        wrapper(auto x) : Wrapper(F(x)) {}
    };

    template <auto F>
    struct convert
    {
        template <typename Wrapper>
        using wrap = wrapper<Wrapper, F>;
    };
} // namespace ct