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

// template v3(C=++14) support rvalue
template<typename Container, typename Index>
decltype(auto) auth_and_access_v3(Container&& c, Index i){

    return std::forward<Container>(c)[i];
}

std::vector<int> make_vector(int size, int value){
    std::vector<int> res(size, value);
    return res;
}

decltype(auto) return_int(){
    int x = 722;
    return x;  //Fine
}

decltype(auto) return_int_reference(){
    int x = 733;
    return (x); //Dangerous, return referecne to a local variable 
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
        std::vector<int> vec(4,4);
        std::cout<< "[Before call auth_and_access] vec[1] : " << vec[1] << '\n';
        auth_and_access<std::vector<int>>(vec, 1) = 100;  // Ok, return int& c
        std::cout<< "[After call auth_and_access] vec[1] : " << vec[1] << '\n';

        std::cout<< "[Before call auth_and_access_v2] vec[2] : " << vec[2] << '\n';
        auth_and_access_v2<std::vector<int>>(vec, 2) = 101;  // Ok, return int& c
        std::cout<< "[After call auth_and_access_v2] vec[2] : " << vec[2] << '\n';

        decltype(auto) t = auth_and_access_v3(std::vector<int>(4,99), 3);  // Ok, return int&
        using erase_t = std::decay_t<decltype(t)>;  // we can use std::decay<T> to erase reference and cv
        erase_t var = 101;
        std::cout << var << '\n';
    }

    {
        //Edge case
        int x = 0;
        //decltype(x); // ok, int
        decltype((x)) dx = x;  // dx is int&
        // If the expression is lvalue, the decltype result will be T&
        // We need to be cautious to use decltype(auto)
        auto var_1 = return_int();
        std::cout << "var_1 " << var_1 << '\n';
        decltype(auto) var_2 = return_int_reference();
        var_2 = 101;  // Dangerous, Undefined behavior
        std::cout << "var_2 " << var_2 << '\n';

    }
    return 0;
}