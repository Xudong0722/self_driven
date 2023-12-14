#include <iostream>
#include <vector>

// decltype 一般用于声明函数模板， 如 我们需要根据函数的参数来确定函数的返回值类型
// template v1
template<typename Container, typename Index>
auto auth_and_access(Container& c, Index i) -> decltype(c[i]){ // 前置的auto表明返回值会进行推导，实际返回值类型在尾置返回类型处进行推导
    // do something
    return c[i];
}

// template v2
template<typename Container, typename Index>
decltype(auto) auth_and_access_v2(Container& c, Index i) { // decltype(auto): auto 表示返回类型将会被推导， decltype表明在推导过程中将会使用decltype
    // do something
    return c[i];
}

int main()
{
    {
        // Basice use of "decltype"
        const int ci = 100;
        decltype(ci) u = 101;  // u : const int

        class Widget{};

        bool f(const Widget& w);

        Widget w;
        decltype(w) dec_w{}; // dec_w : Widget

        decltype(f) dec_f; // dec_f : bool(const )
    }

    {
        std::vector<int> vec(3,3);
        std::cout<< "[Before call auth_and_access] vec[1] : " << vec[1] << '\n';
        auth_and_access<std::vector<int>>(vec, 1) = 100;  // Ok, we return Container& c
        std::cout<< "[After call auth_and_access] vec[1] : " << vec[1] << '\n';

        std::cout<< "[Before call auth_and_access_v2] vec[2] : " << vec[2] << '\n';
        auth_and_access_v2<std::vector<int>>(vec, 2) = 101;  // Ok, we return Container& c
        std::cout<< "[After call auth_and_access_v2] vec[2] : " << vec[2] << '\n';
    }
    return 0;
}