#include <iostream>

enum class machine_status{
    none = 0,
    stop = 1,
    start = 2,
    running = 6,
};

void print(machine_status status){
    switch (status)
    {
    case machine_status::none:
        std::cout << "none\n";
        break;
    case machine_status::stop:
        std::cout << "stop\n";
        break;
    case machine_status::start:
        std::cout << "start\n";
        break;
    case machine_status::running:
        std::cout << "running\n";
        break;
    default:
        std::cout << "no match with " << static_cast<int>(status) << '\n';
        break;
    }
}

int main()
{
    int x = 6;
    auto machine_x = static_cast<machine_status>(x);
    print(machine_x);
    return 0;
}