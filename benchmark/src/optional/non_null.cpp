#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
#include <constrained/constrained_type.hpp>

#include <optional>

namespace nb = ankerl::nanobench;

constexpr std::uint64_t rng_seed = 0xdeadbeef;

namespace manual {
    nb::Rng rng{rng_seed};

    constexpr std::optional<int> deref(int * ptr) noexcept 
    {
        if (ptr) return *ptr;
        return {};
    }
}

namespace constrained {
    nb::Rng rng{rng_seed};

    constexpr auto is_not_null = [](int * ptr) noexcept { return bool(ptr); };
    using non_null_ptr = ct::constrained_type<int *, ct::value_pack<is_not_null>, ct::empty>;
    constexpr std::optional<int> deref(non_null_ptr ptr) noexcept 
    {
        if (ptr) return *ptr.value();
        return {};
    }
}

int main() {
    auto bench = nb::Bench();
    bench
        .relative(true)
        .warmup(100)
        .epochs(1000)
        .minEpochIterations(100'000)
        .run("manual deref", [&]{
            int value = 0;
            int * ptr = manual::rng.bounded(2) ? &value : nullptr;
            nb::doNotOptimizeAway(manual::deref(ptr));
        })
        .run("constrained deref", [&]{
            int value = 0;
            int * ptr = constrained::rng.bounded(2) ? &value : nullptr;
            nb::doNotOptimizeAway(constrained::deref(ptr));
        });
}