#include "threadpool.hpp"
#include <iostream>
#include <mutex>
#include <chrono>

int main() {
    std::mutex cout_mtx;   // keep console output from interleaving
    {
        ThreadPool pool(4);
        for (int i = 0; i < 8; ++i) {
            pool.submit([i, &cout_mtx] {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::lock_guard<std::mutex> lock(cout_mtx);
                std::cout << "Task " << i << " handled by thread "
                          << std::this_thread::get_id() << "\n";
            });
        }
    }   // <-- pool goes out of scope here: destructor drains tasks, joins threads
    std::cout << "All tasks complete.\n";
    return 0;
}