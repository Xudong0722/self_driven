#include <iostream>
#include <type_traits>

template<class T1, class T2>
struct A{
    template<class U1 = T1, class U2 = T2,
    typename std::enable_if<
    std::is_copy_constructible<U1>::value &&
    std::is_copy_constructible<U2>::value &&
    std::is_convertible<const U1&, T1>::value &&
    std::is_convertible<const U2&, T2>::value, int>::type = 0>
    constexpr A(const T1& a, const T2& b)
    : first(a), second(b)
    {}

    T1 first;
    T2 second;

    void print(){
        std::cout << first << " " << second << std::endl;
    }
};

int main()
{
    int a = 101;
    double b = 1.01;

    A<int, double> c(a, b);
    c.print();

    A<double, int> d(2.0, 3);
    d.print();
    return 0;
}