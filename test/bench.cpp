#include "../xoshiro.hpp"
#include "../dice_distribution.hpp"

#include <iostream>
#include <random>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <benchmark/benchmark.h>

using namespace xoshiro;

template <class T>
static void BM_PRNG(benchmark::State& state) {
    T rng(42);
    for (auto _ : state) {
        benchmark::DoNotOptimize(rng());
    }
}

BENCHMARK_TEMPLATE(BM_PRNG, std::mt19937);
BENCHMARK_TEMPLATE(BM_PRNG, std::mt19937_64);

BENCHMARK_TEMPLATE(BM_PRNG, xoshiro256pp);
BENCHMARK_TEMPLATE(BM_PRNG, xoshiro256ss);
BENCHMARK_TEMPLATE(BM_PRNG, xoshiro256p);
BENCHMARK_TEMPLATE(BM_PRNG, xoroshiro128pp);
BENCHMARK_TEMPLATE(BM_PRNG, xoroshiro128ss);
BENCHMARK_TEMPLATE(BM_PRNG, xoroshiro128p);

BENCHMARK_TEMPLATE(BM_PRNG, xoshiro128pp);
BENCHMARK_TEMPLATE(BM_PRNG, xoshiro128ss);
BENCHMARK_TEMPLATE(BM_PRNG, xoshiro128p);
BENCHMARK_TEMPLATE(BM_PRNG, xoroshiro64ss);
BENCHMARK_TEMPLATE(BM_PRNG, xoroshiro64s);

template <class TRng, class TDist>
static void BM_Roll(benchmark::State& state) {
    TRng rng(42);
    TDist dist(3, 6, 1);
    TDist dist1("3d6 + 1");
    TDist dist2("1d100");
    TDist dist3("1d4-1");
    TDist dist4("100d2");
    for (auto _ : state) {
        benchmark::DoNotOptimize(dist(rng));
        benchmark::DoNotOptimize(dist1(rng));
        benchmark::DoNotOptimize(dist2(rng));
        benchmark::DoNotOptimize(dist3(rng));
        benchmark::DoNotOptimize(dist4(rng));
    }
}

BENCHMARK_TEMPLATE(BM_Roll, std::mt19937, dice_distribution<>);
BENCHMARK_TEMPLATE(BM_Roll, xoshiro256pp, dice_distribution<>);
BENCHMARK_TEMPLATE(BM_Roll, xoshiro128pp, dice_distribution<>);
BENCHMARK_TEMPLATE(BM_Roll, xoroshiro64ss, dice_distribution<>);
BENCHMARK_TEMPLATE(BM_Roll, xoshiro256pp, dice_distribution<int, boost::random::uniform_int_distribution<int>>);
BENCHMARK_TEMPLATE(BM_Roll, xoshiro256pp, dice_distribution<int, boost::random::uniform_smallint<int>>);
BENCHMARK_TEMPLATE(BM_Roll, xoshiro128pp, dice_distribution<int, boost::random::uniform_smallint<int>>);
BENCHMARK_TEMPLATE(BM_Roll, xoroshiro64ss, dice_distribution<int, boost::random::uniform_smallint<int>>);

BENCHMARK_MAIN();
