#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
#include <constrained/constrained_type.hpp>

#include <optional>

namespace nb = ankerl::nanobench;

constexpr std::uint64_t rng_seed = 0xdeadbeef;

namespace manual {
    nb::Rng rng{rng_seed};

    constexpr int deref(int * ptr)
    {
        if (!ptr) throw std::logic_error{""};
        return *ptr;
    }
}

namespace constrained {
    nb::Rng rng{rng_seed};

    constexpr auto is_not_null = [](int * ptr) noexcept { return bool(ptr); };
    constexpr auto throw_fail = [](auto &) { throw std::logic_error{""}; };
    using non_null_ptr = ct::constrained_type<int *, ct::value_pack<is_not_null>, ct::value_pack<throw_fail>>;
    constexpr std::optional<int> deref(non_null_ptr ptr)
    {
        return *ptr.value();
    }
}

int main() {
    auto bench = nb::Bench();
    bench
        .relative(true)
        .warmup(100)
        .epochs(1000)
        .minEpochIterations(10'000)
        .run("manual deref", [&]{
            int value = 0;
            int * ptr = manual::rng.bounded(2) ? &value : nullptr;
            try { nb::doNotOptimizeAway(manual::deref(ptr)); } catch (...) {};
        })
        .run("constrained deref", [&]{
            int value = 0;
            int * ptr = constrained::rng.bounded(2) ? &value : nullptr;
            try { nb::doNotOptimizeAway(constrained::deref(ptr)); } catch (...) {}
        });
}