#pragma once

#include <constrained/optimizer/constraints/optimize.hpp>

namespace ct::fail_handlers::pack {
    template <auto Eq>
    using optimize = ct::constraints::pack::optimize<Eq>;
} // namespace ct::fail_handlers::pack
