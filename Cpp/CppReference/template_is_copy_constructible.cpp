#include <iostream>
#include <type_traits>
#include <vector>

class A{
public:
    A() = default;
    A(const A&) = default;
};

class B{
public:
    B() = default;
    B(const B&) = delete;
};

template<class T>
void print(T t){
    std::cout << std::is_copy_constructible<T>::value << std::endl;
}
int main()
{
    double a{1.0};
    print(a);

    std::vector<int> b{};
    print(b);

    A c;
    print(c);

    //B d;
    //print(d);   //error
    std::cout << (std::is_copy_constructible_v<B>) << std::endl;
    return 0;
}