#include <iostream>
#include <thread>
#include <cassert>
#include <algorithm>
#include <vector>
#include <functional>
#include <chrono>
#include <numeric>  

void some_function() {
	int i = 0;
	while (true) {
		std::cout << "some function : " << ++i << '\n';
	}
}

void some_other_function() {
	int i = 0;
	while (true) {
		std::cout << "some other function : " << ++i << '\n';
	}
}

// 2.3 转移线程所有权
void dangerous_use() {
	std::thread t1{ some_function };
	std::thread t2 = std::move(t1);
	t1 = std::thread(some_other_function);
	std::thread t3 = std::move(t2);
	t1 = std::move(t3);  // crash! t1已经持有一个线程运行some_other_function，不能通过赋值舍弃原来的线程
	std::this_thread::sleep_for(std::chrono::seconds(100));
}

//scoped_thread, RAII
class scoped_thread {
private:
	std::thread t;
public:
	scoped_thread(std::thread t_) :
		t(std::move(t_)) {
		if (!t.joinable()) {  //构造时就判断当前线程是否可以汇合
			throw std::logic_error("No thread");
		}
	}
	~scoped_thread() {
		t.join();
	}
	scoped_thread(const scoped_thread&) = delete;
	scoped_thread& operator=(const scoped_thread&) = delete;
};

void do_work(unsigned i) {
	std::cout << "do word " << i << '\n';
}

void f() {
	scoped_thread t(std::thread(do_work, 101));
	std::cout << std::this_thread::get_id() << '\n';
}


//2.4 运行时决定线程数量
template<typename Iterator, typename T>
struct accumulate_block {
	void operator()(Iterator first, Iterator second, T& result) {
		result = std::accumulate(first, second, result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
	//1.确定是否需要计算
	const auto length = std::distance(first, last);
	if (0 == length) return init;

	//2.设置一个线程的最小任务数
	unsigned int const min_tasks_per_thread = 100;
	
	//3.计算我们理论最大线程数
	unsigned int const max_threads = (length + min_tasks_per_thread - 1) / min_tasks_per_thread;
	//4.计算我们的实际最大线程数
	unsigned int const hardware_threads = std::thread::hardware_concurrency();
	unsigned int nums_threads = std::min(max_threads, hardware_threads == 0 ? 2 : hardware_threads);

	//5.每个线程计算的block大小
	auto const block_size = length / nums_threads;

	std::vector<T> results(nums_threads);
	std::vector<std::thread> threads(nums_threads - 1); // 去掉主线程

	Iterator block_start = first;
	for (decltype(nums_threads) i = 0; i < nums_threads - 1; ++i) {
		Iterator block_end = first;
		std::advance(block_end, block_size);
		threads[i] = std::thread{ accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]) };
		block_start = block_end;
	}

	accumulate_block<Iterator, T>()(block_start, last, std::ref(results[nums_threads - 1]));

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

	return std::accumulate(results.begin(), results.end(), init);
}

//2.5 识别线程
std::thread::id master_thread_id;
void some_core_part_of_algorithm()
{
	auto current_thread_id = std::this_thread::get_id();
	if (master_thread_id == current_thread_id) {
		std::cout << "This is master thread!\n";
	}
	else {
		std::cout << "This thread is not master thread, thread id is: " << current_thread_id << '\n';
	}
}

void day_02() {
	//dangerous_use();
	f();
	std::cout << std::this_thread::get_id() << '\n';
	auto max_thread_nums = std::thread::hardware_concurrency();
	std::cout << max_thread_nums << '\n';
}

void normal_accumulate_version() 
{
	std::vector<double> arr_float(1000000000, 3.1414);
	double result{ 0.0 };

	auto start = std::chrono::high_resolution_clock::now();
	result = std::accumulate(arr_float.begin(), arr_float.end(), 0.0);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "normal version: " << result << ", time: " << duration.count() << '\n';
}

void parallel_accumulate_version() 
{
	std::vector<double> arr_float(1000000000, 3.1414);
	double result{ 0.0 };

	auto start = std::chrono::high_resolution_clock::now();
	result = parallel_accumulate(arr_float.begin(), arr_float.end(), 0.0);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "parallel version: " << result << ", time: " << duration.count() << '\n';
}

void compare_different_version()
{
	normal_accumulate_version();

	parallel_accumulate_version();
}

int main()
{
	//day_02();

	master_thread_id = std::this_thread::get_id();
	std::thread test_thread{ some_core_part_of_algorithm };
	test_thread.join();
	
	some_core_part_of_algorithm();
	return 0;
}