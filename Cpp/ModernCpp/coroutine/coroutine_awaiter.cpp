#include <iostream>
#include <coroutine>
#include <thread>
#include <chrono>

struct Reader
{
    bool await_ready()
    {
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle)
    {
        std::thread worker([this, handle]{
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            value = std::rand();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            handle.resume();
        });

        worker.detach();
    }


    int await_resume()
    {
        return value;
    }

    int value{0};
};

int main()
{
    std::cout << 1 << std::endl;
    std::cout << co_await Reader{} << std::endl;
    std::cout << 2 << std::endl;
}