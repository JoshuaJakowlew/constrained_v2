#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
#include <constrained/constrained_type.hpp>

#include <optional>
#include <vector>

namespace nb = ankerl::nanobench;

constexpr std::uint64_t rng_seed = 0xdeadbeef;

namespace manual {
    nb::Rng rng{rng_seed};

    constexpr std::optional<int> value_at(std::vector<int> const & vec, int i) noexcept 
    {
        if (0 <= i && i < vec.size()) return vec[i];
        return {};
    }
}

namespace constrained {
    nb::Rng rng{rng_seed};

    struct vector_with_index {
        std::vector<int> const & vec;
        int i;
    };

    constexpr auto in_range = [](vector_with_index const & x) noexcept { return 0 <= x.i && x.i < x.vec.size(); };
    using valid_index = ct::constrained_type<vector_with_index, ct::value_pack<in_range>, ct::empty>;
    constexpr std::optional<int> value_at(valid_index i) noexcept 
    {
        if (i) return i->vec[i->i];
        return {};
    }
}

int main() {
    std::vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto bench = nb::Bench();
    bench
        .relative(true)
        .warmup(100)
        .epochs(1000)
        .minEpochIterations(100'000)
        .run("manual value_at", [&]{
            int i = manual::rng.bounded(20);
            nb::doNotOptimizeAway(manual::value_at(vec, i));
        })
        .run("constrained value_at", [&]{
            int i = constrained::rng.bounded(20);
            nb::doNotOptimizeAway(constrained::value_at({vec, i}));
        });
}