/*
 * @Author: Xudong0722
 * @Date: 2024-11-07 23:23:00
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-07 23:56:34
 */

#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <chrono>

int main()
{
    ThreadPool thread_pool(3);

    std::vector<std::future<int>> results;

    for (int i = 1; i <= 9; ++i)
    {
        results.emplace_back(
            thread_pool.add_task([i]()
                                 {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i * i; }));
    }

    for (auto &&result : results)
    {
        std::cout << result.get() << ' ';
    }
    return 0;
}