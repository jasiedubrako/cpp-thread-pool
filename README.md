# cpp-thread-pool

A fixed-size worker thread pool in modern C++ (C++17), built to explore
practical concurrency: worker threads, a shared task queue, and the
synchronization primitives (mutex, condition variable) that make it safe.

## Status
In progress — building up level by level.

## Build
```bash
g++ -std=c++17 -pthread src/main.cpp -o pool
./pool
```

## Roadmap
- [x] Threads 101
- [x] Shared task queue
- [x] Mutex-protected queue
- [x] Condition-variable signalling
- [x] ThreadPool class
- [x] Benchmark vs. sequential execution
- [ ] Graceful shutdown
- [ ] Futures via std::packaged_task

## Benchmark results
On a dual-core machine (4 hardware threads), counting primes below 3,000,000 ran ~1.9× faster with the pool (0.60s → 0.31s) than sequentially, measured with Google Benchmark. Speedup scales with physical core count.