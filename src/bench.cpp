#include "threadpool.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <thread>
#include <cstdlib>

// A deliberately CPU-bound job: primality by trial division.
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

int main(int argc, char** argv) {
    int N = (argc > 1) ? std::atoi(argv[1]) : 3000000;   // runtime input defeats constant-folding
    using clock_type = std::chrono::steady_clock;

    // --- Sequential baseline ---
    auto s0 = clock_type::now();
    int seq_count = count_primes(0, N);
    auto s1 = clock_type::now();
    double seq_secs = std::chrono::duration<double>(s1 - s0).count();

    // --- Parallel, via the pool ---
    unsigned int hw = std::thread::hardware_concurrency();
    unsigned int num_threads = hw ? hw : 4;
    const int num_chunks = num_threads * 4;              // more chunks than threads = better load balance
    std::vector<int> results(num_chunks, 0);
    int chunk = (N + num_chunks - 1) / num_chunks;       // ceiling division

    auto p0 = clock_type::now();
    {
        ThreadPool pool(num_threads);
        for (int c = 0; c < num_chunks; ++c) {
            int lo = c * chunk;
            int hi = std::min(N, lo + chunk);
            pool.submit([lo, hi, c, &results] {
                results[c] = count_primes(lo, hi);       // each task writes its OWN slot
            });
        }
    }   // pool destructor waits for all tasks to finish
    int par_count = 0;
    for (int r : results) par_count += r;
    auto p1 = clock_type::now();
    double par_secs = std::chrono::duration<double>(p1 - p0).count();

    std::cout << "Hardware threads: " << hw << " | pool size: " << num_threads << "\n";
    std::cout << "Primes (sequential): " << seq_count << "  |  (parallel): " << par_count << "\n";
    std::cout << "Sequential time: " << seq_secs << " s\n";
    std::cout << "Parallel time:   " << par_secs << " s\n";
    std::cout << "Speedup:         " << (seq_secs / par_secs) << "x\n";
    return 0;
}