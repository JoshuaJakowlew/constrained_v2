#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <iostream>

#include <manual.hpp>
#include <constrained.hpp>

namespace nb = ankerl::nanobench;

using namespace ct::bench;

nb::Rng manual_rng;
nb::Rng constrained_rng;

void deref_manual()
{
    std::unique_ptr<int> ptr{
        manual_rng() % 2 == 0 ? new int{42} : nullptr
    };
    try {
        nb::doNotOptimizeAway(manual::deref_ptr_throwing(std::move(ptr)));
    } catch (std::logic_error const &) {}
}

void deref_constrained()
{
    std::unique_ptr<int> ptr{
        constrained_rng() % 2 == 0 ? new int{42} : nullptr
    };
    try {
        nb::doNotOptimizeAway(constrained::deref_ptr_throwing(std::move(ptr)));
    } catch (std::logic_error const &) {}
}

int main() {
    // deref_manual();
    // deref_constrained();

    std::string parse_int_data[]{
        "1",
        "12",
        "123",
        "1b3",
        "10005000",
        ""
    };

    auto bench_is_even = nb::Bench();
    bench_is_even
        .warmup(100)
        .epochs(1000)
        .minEpochIterations(100'000)
        .run("manual even", [&]{
            const auto x = manual_rng();
            
            nb::doNotOptimizeAway(manual::even(x));
        })
        .run("constrained even", [&]{
            const auto x = constrained_rng();
            nb::doNotOptimizeAway(constrained::even(x));
        });

    auto bench_deref = nb::Bench();
    bench_deref
        .warmup(100)
        .epochs(100)
        .minEpochIterations(100'000)
        .run("manual deref", [&]{
            std::unique_ptr<int> ptr{
                manual_rng() % 2 == 0 ? new int{42} : nullptr
            };
            nb::doNotOptimizeAway(manual::deref_ptr(std::move(ptr)));
        })
        .run("constrained deref", [&]{
            std::unique_ptr<int> ptr{
                constrained_rng() % 2 == 0 ? new int{42} : nullptr
            };
            nb::doNotOptimizeAway(constrained::deref_ptr(std::move(ptr)));
        });

    auto bench_throwing_deref = nb::Bench();
    bench_throwing_deref
        .warmup(100)
        .epochs(100)
        .minEpochIterations(10'000)
        .run("manual deref throwing", [&]{
            std::unique_ptr<int> ptr{
                manual_rng() % 2 == 0 ? new int{42} : nullptr
            };
            try {
                nb::doNotOptimizeAway(manual::deref_ptr_throwing(std::move(ptr)));
            } catch (std::logic_error const &) {}
        })
        .run("constrained deref throwing", [&]{
            std::unique_ptr<int> ptr{
                constrained_rng() % 2 == 0 ? new int{42} : nullptr
            };
            try {
                nb::doNotOptimizeAway(constrained::deref_ptr_throwing(std::move(ptr)));
            } catch (std::logic_error const &) {}
        });

    auto bench_parse_int = nb::Bench();
    bench_parse_int
        .warmup(100)
        .epochs(100)
        .minEpochIterations(10'000)
        .run("manual parse_int", [&]{
            const int i = manual_rng() % 6;
            try {
                nb::doNotOptimizeAway(manual::parse_int(parse_int_data[i]));
            } catch (std::logic_error const &) {}
        })
        .run("constrained even", [&]{
            const int i = constrained_rng() % 6;
            try {
                nb::doNotOptimizeAway(constrained::parse_int(parse_int_data[i]));
            } catch (std::logic_error const &) {}
        });
}