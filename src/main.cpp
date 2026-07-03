#include <iostream>
#include <thread>
#include <vector>

void worker(int id) {
    std::cout << "Worker " << id << " reporting in\n";
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 8; ++i) {
        threads.emplace_back(worker, i);   // start 8 threads, each running worker(i)
    }

    for (auto& t : threads) {
        t.join();                          // wait for all of them to finish
    }

    std::cout << "All workers done.\n";
    return 0;
}