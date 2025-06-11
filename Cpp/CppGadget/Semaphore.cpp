#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>

class Seamphore {
public:
    Seamphore(int cnt) : count_(cnt) {}
    ~Seamphore() {}

    void acquire() {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [this]{ return count_ > 0;});
        --count_;
    }

    void release() {
        std::unique_lock<std::mutex> lk(mutex_);
        ++count_;
        cv_.notify_one();
    }

private:
    std::mutex mutex_;
    int count_{0};
    std::condition_variable cv_;
};

Seamphore sem(1);
int i = 0;
void fun1() {
    for(; i < 20; ++i) {
        sem.acquire();
        std::cout << "fun1--" << i << std::endl;
        sem.release();
    }
}

void fun2() {
    for(; i < 20; ++i) {
        sem.acquire();
        std::cout << "fun2--" << i << std::endl;
        sem.release();
    }
}

int main() {
    std::thread t1(fun1);
    std::thread t2(fun2);
    t1.join();
    t2.join();
    return 0;
}