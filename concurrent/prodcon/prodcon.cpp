#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

using namespace std;

static volatile bool isDone = false;

mutex queue_access;
mutex wake_up;

condition_variable cv;
queue<int> que;

void producer(int id) {
    cout << "Producer Starting...\n";

    int i = 0;
    int count = 0;

    while (!isDone) {
        this_thread::sleep_for(chrono::milliseconds(250));

        {
            lock_guard<mutex> lk(queue_access);
            int n = (41 * count) % 13 + 1;
            for (int j = 0; j < n; j++) {
                que.push(i++);
            }
            count = (count + 1) % 1000000;
        }

        cv.notify_one();
    }
    cout << "Producer #" << id << " shutting down." << endl;
}

void consumer(int id) {
    cout << "Consumer #" << id << " Starting...\n";

    int i = 0;
    do {
        unique_lock<mutex> lk(wake_up);
        cv.wait(lk, [] { return !que.empty() || isDone; });

        if (!que.empty()) {
            lock_guard<mutex> lk2(queue_access);
            i = que.front();
            que.pop();
            cout << "Cosumer #" << id << ": " << i << ", "
                 << "queue size: " << que.size() << "\n";
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        lk.unlock();
    } while (!isDone);
    
    cout << "Consumer #" << id << " shutting down." << endl;
}

int main() {
    thread p[3];
    for(int i=0; i<3; i++){
	p[i] = thread(&producer, i);
    }

    thread c[10];
    for (int i = 0; i < 10; i++) {
        c[i] = thread(&consumer, i);
    }

    this_thread::sleep_for(chrono::milliseconds(10*1000));
    cout << "Terminating..." << endl;
    isDone = true;

    for(int i=0; i<3; i++){
	p[i].join();
    }
    cv.notify_all();
    for (int i = 0; i < 10; i++) {
        c[i].join();
    }
    return 0;
}

