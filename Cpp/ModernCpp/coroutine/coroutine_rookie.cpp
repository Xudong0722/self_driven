#include <coroutine>
#include <iostream>

struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    handle_type coro_handle;

    Generator(handle_type h) : coro_handle(h) {}
    ~Generator() { if (coro_handle) coro_handle.destroy(); }

    bool move_next() {
        coro_handle.resume();
        return !coro_handle.done();
    }

    int current_value() { return coro_handle.promise().current_value; }

    struct promise_type {
        int current_value;

        auto get_return_object() {
            return Generator{handle_type::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

Generator simple_generator() {
    for (int i = 0; i < 5; ++i) {
        co_yield i;
    }
}

int main() {
    auto gen = simple_generator();

    while (gen.move_next()) {
        std::cout << "Generated value: " << gen.current_value() << std::endl;
    }
}
