#include "threadpool.hpp"
#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <thread>

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; (long long)i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}
int count_primes(int lo, int hi) {
    int c = 0;
    for (int n = lo; n < hi; ++n)
        if (is_prime(n)) ++c;
    return c;
}

static const int N = 3000000;

static void BM_Sequential(benchmark::State& state) {
    for (auto _ : state) {
        int c = count_primes(0, N);
        benchmark::DoNotOptimize(c);          // stop the optimizer deleting the work
    }
}
BENCHMARK(BM_Sequential)->Unit(benchmark::kMillisecond);

static void BM_Parallel(benchmark::State& state) {
    unsigned int hw = std::thread::hardware_concurrency();
    unsigned int num_threads = hw ? hw : 4;
    const int num_chunks = num_threads * 4;
    int chunk = (N + num_chunks - 1) / num_chunks;
    for (auto _ : state) {
        std::vector<int> results(num_chunks, 0);
        {
            ThreadPool pool(num_threads);
            for (int c = 0; c < num_chunks; ++c) {
                int lo = c * chunk, hi = std::min(N, lo + chunk);
                pool.submit([lo, hi, c, &results] { results[c] = count_primes(lo, hi); });
            }
        }
        int total = 0; for (int r : results) total += r;
        benchmark::DoNotOptimize(total);
    }
}
BENCHMARK(BM_Parallel)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();