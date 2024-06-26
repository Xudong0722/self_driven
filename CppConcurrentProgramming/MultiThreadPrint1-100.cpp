#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <condition_variable>

int num{ 1 };
bool flag{ true };
std::mutex num_mutex{};
std::condition_variable cv{};


void printNumA() {
	
	for (; num <= 100; ++num) {
		std::unique_lock<std::mutex> lk(num_mutex);
		cv.wait(lk, [] {return flag;});
		std::cout << std::this_thread::get_id() << " " << num << '\n';
		flag = !flag;
		cv.notify_one();
	}
}

void printNumB() {
	for (; num <= 100; ++num) {
		std::unique_lock<std::mutex> lk(num_mutex);
		cv.wait(lk, [] {return !flag;});
		std::cout << std::this_thread::get_id() << " " << num << '\n';
		flag = !flag;
		cv.notify_one();
	}
}

int main()
{
	std::thread t1(printNumA);
	std::thread t2(printNumB);
	t1.join();
	t2.join();
	return 0;
}