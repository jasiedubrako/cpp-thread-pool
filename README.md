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
- [ ] Shared task queue
- [ ] Mutex-protected queue
- [ ] Condition-variable signalling
- [ ] ThreadPool class
- [ ] Benchmark vs. sequential execution
- [ ] Graceful shutdown
- [ ] Futures via std::packaged_task

## Benchmark results
_Coming soon._