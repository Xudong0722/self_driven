/*
 * @Author: Xudong0722
 * @Date: 2024-10-29 23:52:53
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-08 00:00:33
 */

#include "ThreadPool.h"
#include <future>

ThreadPool::ThreadPool(int size)
    : stop_(false)
{
    for (int i = 0; i < size; ++i)
    {
        threads_.emplace_back(
            [this]()
            {
                for (;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex_);
                        condition_.wait(lock, [this]
                                        { return this->stop_ || !this->tasks_.empty(); });
                        if (this->stop_ && this->tasks_.empty())
                            return false;
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    task();
                }
            });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        this->stop_ = true;
    }
    condition_.notify_all();
    for (auto &thread : this->threads_)
    {
        if (thread.joinable())
            thread.join();
    }
}
