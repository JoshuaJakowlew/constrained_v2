#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
#include <constrained/constrained_type.hpp>

#include <optional>
#include <vector>

namespace nb = ankerl::nanobench;

constexpr std::uint64_t rng_seed = 0xdeadbeef;

namespace manual {
    nb::Rng rng{rng_seed};

    constexpr std::optional<int> value_at(std::vector<int> const & vec, int i)
    {
        if (!(0 <= i && i < vec.size())) throw std::logic_error{""};
        return vec[i];
    }
}

namespace constrained {
    nb::Rng rng{rng_seed};

    struct vector_with_index {
        std::vector<int> const & vec;
        int i;
    };

    constexpr auto in_range = [](vector_with_index const & x) noexcept { return 0 <= x.i && x.i < x.vec.size(); };
    constexpr auto throw_fail = [](auto &) { throw std::logic_error{""}; };
    using valid_index = ct::constrained_type<vector_with_index, ct::value_pack<in_range>, ct::value_pack<throw_fail>>;
    constexpr std::optional<int> value_at(valid_index i) noexcept 
    {
        return i->vec[i->i];
    }
}

int main() {
    std::vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto bench = nb::Bench();
    bench
        .relative(true)
        .warmup(100)
        .epochs(1000)
        .minEpochIterations(10'000)
        .run("manual value_at", [&]{
            int i = manual::rng.bounded(20);
            try { nb::doNotOptimizeAway(manual::value_at(vec, i)); } catch (...) {}
        })
        .run("constrained value_at", [&]{
            int i = constrained::rng.bounded(20);
            try { nb::doNotOptimizeAway(constrained::value_at({vec, i})); } catch (...) {}
        });
}