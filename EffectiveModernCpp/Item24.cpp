#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>

//1.函数模板形参
template<typename T>
void f(T&& param){ // param是一个通用引用

}

template<typename T>
void fa(const T&& param){ //param 是一个右值引用， const T&& 只能绑定右值，无法绑定左值

}

template<typename T>
void fb(std::vector<T>&& param){ //param是右值引用

}

namespace elvis{
    template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class vector{
    public:
    #if __cplusplus >= 201103L
        void
        push_back(_Tp&& __x)    //这里并不是通用引用， 因为vector实例化的时候_Tp已经知道，不需要再进行推导
        { emplace_back(std::move(__x)); }

        template<typename... _Args>
        void
        emplace_back(_Args&&... __args);  //是通用引用
    #endif
    };
}

int fib(int n){
    if(n == 0 || n == 1) return n;
    return fib(n-1) + fib(n-2);
}

int main()
{

    /*
    Item24.区分通用引用和右值引用
    1.函数模板形参
    2.auto 声明符
    
    note: 形如T&&并且发生了类型推导，那么这个形参就是通用引用
    */

    int a{0}, b{1}, c{2};
    //std::vector<int&> v{a, b, c};   //引用不是一个对象，他只是一个变量的别名



    //auto&&
    {
        auto cal_exe_time = [](auto && func, auto &&... param){   // 这里也是通用引用
            const auto start_time{std::chrono::steady_clock::now()};
            auto ans = std::forward<decltype(func)>(func)(std::forward<decltype(param)>(param)...);
            const auto end_time{std::chrono::steady_clock::now()};
            const auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
            // const std::chrono::duration<double> elapsed_seconds{end_time - start_time};
            std::cout << ans << " " << elapsed_milliseconds.count() << '\n';
        };

        std::cout << "input a big number:";
        int big_number{0};
        std::cin >> big_number;
        cal_exe_time(fib, big_number);
    }

    return 0;
}