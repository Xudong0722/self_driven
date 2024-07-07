#define _GLIBCXX_DEBUG 
#include <iostream>
#include <memory>

constexpr int func()
{
    return 1 << 31;
}

constexpr int funb(){
    int arr[10]{};
    return arr[0];
}

int main()
{
    std::string s{"hello"};
    std::string s2 = std::move(s);
    std::cout << fund() << std::endl;

    memcpy()
    return 0;
}