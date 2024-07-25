#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <iostream>

#include <manual.hpp>
#include <constrained.hpp>

namespace nb = ankerl::nanobench;

using namespace ct::bench;

int main() {
    nb::Rng manual_rng;
    nb::Rng constrained_rng;

    auto bench_non_null = nb::Bench();
    bench_non_null
        .warmup(100)
        .epochs(100)
        .minEpochIterations(100'000)
        .run("manual even", [&]{
            const auto x = manual_rng();
            
            nb::doNotOptimizeAway(manual::even(x));
        })
        .run("constrained even", [&]{
            const auto x = constrained_rng();
            nb::doNotOptimizeAway(constrained::even(x));
        })
        .run("manual even", [&]{
            int * x = new int(manual_rng());
            std::unique_ptr<int> ptr{x};
            nb::doNotOptimizeAway(manual::deref_ptr(std::move(ptr)));
        })
        .run("constrained even", [&]{
            int * x = new int(constrained_rng());
            std::unique_ptr<int> ptr{x};
            nb::doNotOptimizeAway(constrained::deref_ptr(std::move(ptr)));
        });
        
}