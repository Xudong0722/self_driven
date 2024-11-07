/*
 * @Author: Xudong0722
 * @Date: 2024-10-28 23:59:47
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-10-29 23:20:32
 */

#include <iostream>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <string>

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread()
{
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, []
            { return ready; });

    std::cout << "Worker thread is processing data.\n";
    data += " after processing";

    // processed = true;
    std::cout << "Worker thread  signals data processing completed \n";

    lock.unlock();
    cv.notify_one();
}

int main()
{
    std::thread worker(worker_thread);

    data = "Example data";
    {
        std::lock_guard<std::mutex> lock(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();

    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, []
                { return processed; });
    }
    std::cout << "Back in main(), data = " << data << std::endl;

    worker.join();
    return 0;
}