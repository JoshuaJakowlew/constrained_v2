#pragma once

#include <constrained/optimizer/constraints/optimize.hpp>

namespace ct::validators::pack {
    template <auto Eq>
    using optimize = ct::constraints::pack::optimize<Eq>;
} // namespace ct::validators::pack
