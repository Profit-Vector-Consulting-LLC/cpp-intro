#include <condition_variable>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>

using namespace std;


int queSize = 20;
static volatile bool isDone = false;

mutex queue_access;
mutex wake_up;

condition_variable cv;
queue<int> que;


void producer() {
        cout << "Producer Starting...\n";

        int i = 0;
	int count = 0;
        while(!isDone) {
		this_thread::sleep_for(chrono::milliseconds(250));
                lock_guard<mutex> lk(queue_access);
		int n = (41*count)%13+1;
		for(int j=0; j<n; j++){
		  que.push(i++);
		}
		count = (count+1)%1000000;
                cv.notify_one();
        }

}

void consumer(int id) {
        cout << "Consumer #" << id << " Starting...\n";
        int i = 0;
        do  {
                unique_lock<mutex> lk(wake_up);
                cv.wait(
                                lk,
                                [] {
                                        return !que.empty() || isDone;
                                }
                        );

                cout << "polling..." << endl;
                if(!que.empty()) {
			lock_guard<mutex> lk2(queue_access);
                        i = que.front();
                        que.pop();
                        cout << "Cosumer #" << id << ": " << i << ", " << "queue size: " << que.size() << "\n";
		        this_thread::sleep_for(chrono::milliseconds(1));
                }
                lk.unlock();
        } while(!isDone);

}

int main() {

        thread p(&producer);

	thread c[10];
        for(int i = 0; i < 10; i++)
                c[i] = thread(&consumer, i);

        p.join();
        for(int i = 0; i < 3; i++)
                c[i].join();


        isDone = true;

        return 0;
}


