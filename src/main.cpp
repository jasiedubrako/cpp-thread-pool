#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

queue<int> tasks;
mutex mtx;
condition_variable cv;
bool done = false;
int processed = 0;

void worker() {
    while(true) {
        int task;
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, []{ return !tasks.empty() || done; });
            if (tasks.empty() && done) return;
            task = tasks.front();
            tasks.pop();
            processed++;
        }
        (void)task;
    }
}


int main() {
    vector<thread> threads;
    for (int i = 0; i < 4; i++) {
        threads.emplace_back(worker);
    }

    this_thread::sleep_for(chrono::seconds(2));

    for (int i = 0; i < 8; ++i) {
        lock_guard<mutex> lock(mtx); tasks.push(i);
        cv.notify_one();
    }

    {
        lock_guard<mutex> lock(mtx); done = true;
        cv.notify_all();
    }

    for (auto& t : threads) t.join();

    cout << "Processed: " << processed << "\n";

    return 0;
}