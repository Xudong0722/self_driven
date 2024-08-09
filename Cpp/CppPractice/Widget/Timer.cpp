#include <iostream>
#include <chrono>
#include <thread>

class Timer {
public:
    Timer(int64_t _time_required = 10) : time_required(_time_required) {
        tp = std::chrono::steady_clock::now();
        std::cout << std::this_thread::get_id() << " Start: 0s\n";
        while (true) {
            OnIdle();
            if (sec == _time_required) {
                std::cout << std::this_thread::get_id() << " Task complete";
                break;
            }
        }
    }
    ~Timer() = default;
private:
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

public:
    void OnIdle() {
        const auto cur = std::chrono::steady_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::seconds>(cur - tp);
        if (duration == std::chrono::seconds(1)) {
            sec++;
            std::cout << std::this_thread::get_id() << " " <<  sec << "s\n";
            tp = cur;
        }
    }
private:
    std::chrono::steady_clock::time_point tp;
    int64_t sec{ 0 };
    int64_t time_required{ 0 };

};

void func1() {
    std::cout << "Task func1\n";
    Timer t(20);
}

void func2() {
    std::cout << "Task func2\n";
    Timer t(30);
}

void func3() {
    int a{ 0 };
    std::cin >> a;
    std::cout << a << std::endl;
}

int main()
{

    std::thread t1(func1);

    std::thread t2(func2);

    std::thread t3(func3);

    t1.join();
    t2.join();
    t3.join();
    return 0;
}

 