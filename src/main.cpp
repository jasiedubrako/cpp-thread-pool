#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>

using namespace std;

queue<int> tasks;
int processed = 0;
mutex mtx;

void worker() {
    while(true) {
        int task;
        {
            lock_guard<mutex> lock(mtx);
            if (tasks.empty()) return;
            task = tasks.front();
            tasks.pop();
            processed++;
        }
        (void)task;
    }
}


int main() {
    const int N = 100000;
    for (int i = 0; i < N; ++i) tasks.push(i);
    vector<thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "Enqueued: " << N << " Processed: " << processed << "\n";
    return 0;
}