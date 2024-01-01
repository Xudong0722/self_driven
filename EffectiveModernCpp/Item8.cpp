#include <iostream>

void f(int x){
    std::cout << "call f(int)\n";
}

void f(bool b){
    std::cout << "call f(bool)\n";
}

void f(void *){
    std::cout << "call f(void*)\n";
}

int main()
{
    f(NULL);  // it will not call f(void *), so it has conflict with its definition.

    f(nullptr);  // Fine!  std::nullptr_t  完美的循环定义，可以隐式转换任何内置类型的指针
    return 0;
}