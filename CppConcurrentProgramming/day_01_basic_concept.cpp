#include <iostream>
#include <thread>
#include <string>

void print_helloworld(std::string str) {
	std::cout << "Hello world!\n" << str << std::endl;
}

class background_task {
public:
	void operator()() const {
		std::cout << "background task is running!\n";
	}
};

struct func {
	int& ref;
	func(int& param) : ref(param) {};
	void operator()() const {
		constexpr int loop_cnt = 5;
		for (int i = 0; i < loop_cnt; ++i) {
			ref = i;
			std::cout << "[func] The ref is " << ref << '\n';
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
};

void test_func() {
	int local_state{ 0 };
	func my_func{ local_state };
	std::thread t3(my_func);
	t3.detach();  //detach. test_func 不会等待t3线程结束，所以local_state会被销毁，my_func中的引用可能会悬空
	// t3.join(); // will fix above issue, but actually it's not detach!!!
}

//special situation, we need to waiting for sub thread execute 
void catch_exception() {
	int local_state{ 0 };
	func my_func{ local_state };
	std::thread t3(my_func);
	try {
		// do something in current thread
		std::cout << "[catch exception] do something in current thread\n";
	}
	catch (std::exception& e) {
		t3.join();  //Although current thread is crash, we need to join thread 3, maybe it's a important event 
		throw e;
	};

	t3.join();
}

//RAII [context manager]
class thread_guard {
private:
	std::thread& m_thread;
public:
	thread_guard(std::thread& t) : m_thread(t) {}
	~thread_guard() {
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	thread_guard(const thread_guard& t) = delete;
	std::thread& operator=(const thread_guard& t) = delete;
};

void auto_guard() {
	int local_state{ 0 };
	func my_func{ local_state };
	std::thread t3(my_func);
	thread_guard g(t3);
	//do something in current thread
	std::cout << "[auto guard] do something in current thread\n";
}

void change_name(std::string& name) {
	name = "elvis!!!";
}

void oops(std::string name) {
	std::cout << "[oops] name is " << name << '\n';
	//std::thread t(change_name, name);  // pass right value to left-ref will cause error
	std::thread t(change_name, std::ref(name)); //right, there is a reference wrapper
	t.join();
	std::cout << "[oops] name is " << name << '\n';
}

// bind member function to thread
class X {
public:
	void print_x(int x) const {
		std::cout << "[X::print_x] X " << x << '\n';
	}
};

void bind_member_function() {
	std::cout << "[bind_member_function] begin\n";
	X my_x;
	std::thread t(&X::print_x, &my_x, 101);
	t.join();
	std::cout << "[bind_member_function] end\n";
}

// Special situation, when the parammeter can't copiable, we can use std::move operation
void process_x_object(std::unique_ptr<int> u_ptr) {
	std::cout << "x is " << *u_ptr << '\n';
	*u_ptr = 722;
	std::cout << "x is " << *u_ptr << '\n';
}

void process_noncopyable_object() {
	std::cout << "[process_noncopyable_object] begin\n";
	std::unique_ptr<int> uptr{ new int(101) };
	//std::thread t(process_x_object, uptr); // error, unique ptr can't copy
	std::thread t(process_x_object, std::move(uptr)); // we can pass control by std::move()
	t.join();
	std::cout << "[process_noncopyable_object] end\n";
}

int main()
{
	std::string name{ "Elvis" };
	std::thread t1(print_helloworld, name);
	t1.join();
	
	//functor
	background_task f;
	std::thread t2(f);
	t2.join();
	
	//Notice, the following initialization will cause build error
	//std::thread t_functor_error(background_task());
	//t_functor_error.join(); //expression must have class type but it has type "std::thread (*)(background_task (*)())"
	
	//The following methods will resolve this problem
	std::thread t3((background_task()));
	t3.join();

	std::thread t4{ background_task() };
	t4.join();

	// Lambda initialization
	std::thread t5{ [](const std::string str) {
		std::cout << "Lambda!!!" << str << std::endl;
	}, name };
	t5.join();    

	//test_func(); // not recommand
	// 防止主线程退出过快，子线程没有运行就crash
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// one thread only join once!
	std::cout << std::boolalpha << t1.joinable() << " " << t2.joinable() << std::endl;

	// Method 1: catch exception to avoid main thread crash when sub thread not execute successfully
	catch_exception();
	
	//Method 2: RAII, thread managed by thread_guard
	auto_guard();

	// Pass parameter to thread function
	oops(name);
	
	bind_member_function();

	process_noncopyable_object();
	return 0;
}
