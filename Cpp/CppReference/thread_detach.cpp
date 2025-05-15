#include <iostream>
#include <thread>

int main()
{

    std::thread worker([]
                       { std::cout << "worker thread\n"; });
    std::cout << "main thread\n";
    worker.join();
    worker.detach();
    return 0;
}