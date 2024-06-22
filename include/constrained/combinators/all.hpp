#pragma once

namespace ct {
    template <auto... Predicates>
    struct all
    {
        using combinator_tag = void;
        
        constexpr bool operator()(auto const & value) const
            noexcept(noexcept((... && Predicates(value))))
        {
            return (... && Predicates(value));
        }
    };
} // namespace ct