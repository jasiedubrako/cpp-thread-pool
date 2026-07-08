# Concurrent Thread Pool

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B&logoColor=white)
![Build](https://img.shields.io/badge/build-g%2B%2B%20--pthread-brightgreen)
![Race--free](https://img.shields.io/badge/ThreadSanitizer-0%20races-success)

A fixed-size worker thread pool in modern C++ (C++17). Worker threads pull tasks
from a shared, mutex-protected queue and sleep efficiently on a condition variable
when idle — the core concurrency pattern behind high-throughput systems.

## Features
- Fixed pool of worker threads, created once and reused
- Thread-safe task queue (`std::mutex` + `std::condition_variable`)
- Generic `submit()` accepting any `std::function<void()>` task
- RAII lifecycle: workers start on construction, drain and join on destruction
- Verified race-free with ThreadSanitizer

## Usage
```cpp
#include "threadpool.hpp"

ThreadPool pool(4);                 // 4 worker threads
pool.submit([] { /* do work */ });  // hand off a task
// pool drains all tasks and joins threads when it goes out of scope
```

## Build
```bash
g++ -std=c++17 -pthread -O2 src/main.cpp -o pool && ./pool
```

## Benchmark
Counting primes below 3,000,000, sequential vs. pool (Google Benchmark):

| Execution   | Time   | Speedup |
|-------------|--------|---------|
| Sequential  | 381 ms | 1.0×    |
| Thread pool | 79 ms  | ~4.8×   |

Measured across 12 hardware threads on a 10-core hybrid CPU (Intel i7-1365U).
Speedup scales with available cores.

## Roadmap
- [x] Thread basics, shared queue, mutex, condition variable
- [x] Reusable `ThreadPool` class
- [x] Benchmark vs. sequential execution
- [ ] Graceful shutdown refinements
- [ ] Result-returning tasks via `std::packaged_task`
