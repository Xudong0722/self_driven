#include <iostream>
#include <cstdint>

int main()
{
    int32_t a = 10;
    uint32_t r_a = ~a;
    std::cout << a << "\n" << r_a << "\n";

    uint32_t b = 10;
    uint32_t r_b = ~b;
    std::cout << b << "\n" << r_b << "\n";
    return 0;
}