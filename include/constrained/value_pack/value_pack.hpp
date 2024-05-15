#pragma once

#include <cstddef>

namespace ct {
    template <auto... Xs>
    struct value_pack final
    {
    public:
        /*
         * Amount of values in a pack
         */
        static constexpr std::size_t size = sizeof...(Xs);

        template <auto F>
        using map = value_pack<F(Xs)...>;

        template <class F>
        using then = F::template type<Xs...>;
    };

} // namespace ct