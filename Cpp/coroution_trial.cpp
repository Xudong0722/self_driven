#include <iostream>
#include <coroutine>

struct CoResult {
	struct promise_type {
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		CoResult get_return_object() {
			return { std::coroutine_handle<promise_type>::from_promise(*this) };
		}
		void unhandled_exception() {}
		void return_void() {}
	};
	std::coroutine_handle<promise_type> handle;
};

struct Waiter {
	bool await_ready() {
		return false;  
	}
	void await_suspend(std::coroutine_handle<CoResult::promise_type> coroutine_handle) {

	}

	void await_resume() {
	}
};

CoResult outputHelloEast() {
	std::cout << "Hello";
	co_await Waiter{};
	std::cout << "East!";
}

int main()
{
	auto co_result = outputHelloEast();
	std::cout << "oooooooooEEEEEEEEE";
	co_result.handle.resume();
	return 0;
}