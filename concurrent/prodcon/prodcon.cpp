#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

using namespace std;

static const int RUN_DURATION_SECONDS = 15;
static const int PRODUCER_COUNT=30;
static const int PRODUCER_WORKTIME_MILLIS = 250;

static const int CONSUMER_COUNT=10;
static const int CONSUMER_WORKTIME_MILLIS = 1;

static volatile bool isProducerDone = false;
static volatile bool isConsumerDone = false;

mutex queue_access;
mutex wake_up;

condition_variable cv;
queue<int> que;

void producer(int id) {
    cout << "Producer Starting...\n";

    int i = 0;
    int count = 0;

    while (!isProducerDone) {
        this_thread::sleep_for(chrono::milliseconds(PRODUCER_WORKTIME_MILLIS));

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
        cv.wait(lk, [] { return !que.empty() || isProducerDone; });

        if (!que.empty()) {
            lock_guard<mutex> lk2(queue_access);
            do {
                i = que.front();
                que.pop();
                cout << "Consumer #" << id << ": " << i << ", "
                     << "queue size: " << que.size() << "\n";
            } while(isProducerDone && !que.empty()); // If producers done, drain the queue.
            this_thread::sleep_for(chrono::milliseconds(CONSUMER_WORKTIME_MILLIS));
        }

    } while (!isConsumerDone);
    
    cout << "Consumer #" << id << " shutting down." << endl;
}

int main() {
    // Start the producers
    thread p[PRODUCER_COUNT];
    for(int i=0; i<PRODUCER_COUNT; i++){
	p[i] = thread(&producer, i+1);
    }

    // Start the consumers
    thread c[CONSUMER_COUNT];
    for (int i = 0; i < CONSUMER_COUNT;  i++) {
        c[i] = thread(&consumer, i+1);
    }

    // Wait for the alloted time, then terminate.
    this_thread::sleep_for(chrono::milliseconds(RUN_DURATION_SECONDS*1000));

    cout << endl
         << "**************************************************************" << endl
         << "Terminating..." << endl
         << "**************************************************************" << endl;
    // Set flag to end producers, next time producers 
    // iterate, they will exit naturally.  It will also indicate
    // to the consumers that shutdown has begun, and each consumer
    // will now operate to empty the queue whenever it acquires the 
    // condition variable lock.
    isProducerDone = true;

    // Wait for all producers to quit.  They don't block on a condition
    // so they will quit first.  Remaining consumers will then 
    // empty the queue the last time.  Ones that process items will
    // not block and will exit when they see isDebug is true.
    for(int i=0; i<PRODUCER_COUNT; i++){
        p[i].join();
    }
    // At this point the queue is no longer filled.

    // Set the flag to indicate to consumers to quit after processing
    // their last batch of work.
    isConsumerDone = true;

    // Notify all consumers waiting to stop waiting.
    // They will see isConsumerDone is true and will exit naturally too.
    cv.notify_all();

    // Wait for all consumers to quit.
    for (int i = 0; i < CONSUMER_COUNT; i++) {
        c[i].join();
    }
    // At this point it is highly likely but not guaranteed that the
    // queue is empty.

    // Launch one more consumer to ensure that 
    // we have cleared out the queue.  If there's anything
    // left in the queue it will clear it out then immediately
    // terminate. 
    thread cleanup = thread(&consumer, 0);
    cleanup.join();

    // All worker threads exited, terminate.
    return 0;
}

