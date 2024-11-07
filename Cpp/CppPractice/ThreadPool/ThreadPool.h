/*
 * @Author: Xudong0722
 * @Date: 2024-10-28 23:46:09
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-08 00:00:15
 */

#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <future>

#define DEFAULT_POOL_SIZE 5

class ThreadPool
{
public:
    ThreadPool(int size = DEFAULT_POOL_SIZE);
    ~ThreadPool();

    template <typename Func, typename... Args>
    auto add_task(Func &&func, Args &&...args)
        -> std::future<typename std::result_of<Func(Args...)>::type>;

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;

    // sync
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_;
};

template <typename Func, typename... Args>
auto ThreadPool::add_task(Func &&func, Args &&...args)
    -> std::future<typename std::result_of<Func(Args...)>::type>
{
    using return_type = typename std::result_of<Func(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        if (this->stop_)
            throw std::runtime_error("add task on stopped threadpool");
        tasks_.emplace([task]()
                       { (*task)(); });
    }
    condition_.notify_one();
    return res;
}