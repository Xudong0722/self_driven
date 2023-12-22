#include <iostream>
#include <functional>
#include <vector>

void do_something(int x){
    // do something with x
}

class Widget{

};

int main()
{
    {
        //Reason 1: 声明变量时忘记初始化，后续使用这个变量可能造成未定义行为，但是使用auto就必须要有初始值，可避免变量没有初始化。
        //习惯问题，可避免，定义变量的同时要赋予初始值！！！

        int var;
        do_something(var);  // dangerous, the value of var is uncertain value
        
        // auto var2; //error, we need to set initial value to a variable when we use auto
        auto var2 = 101; //OK!
        do_something(var2);
    }


    {
        //Reason 2: 避免写冗余的代码

        std::function<bool(const Widget&, const Widget&)> func = [](const Widget& a, const Widget& b) -> bool{
            //do something
            return true;
        };

        // we can use auto to simplify the above statement
        auto auto_func = [](const Widget& a, const Widget& b) -> bool{
            //do something
            return true;
        };

        //In addition, using auto has another advance here - Save Memory
        //std::function 实例化时会固定和闭包一样的大小，如果这个空间不足以存储这个闭包，std::function的构造函数会在堆上申请内存去构造，
        //所以会比auto慢不少，使用auto的话会用一个和闭包大小一样的内存来存储闭包。(Note， std::function存储可调用对象，是一个模板类，所以才会有空间不足以存储这个闭包的问题)
    }


    {
        //Reason3: 你显示声明的类型可能是错的
        std::vector<int> vec(10,0);
        int vec_size = vec.size();
        
        auto vec_size_auto = vec.size();  // return value type is right when we use auto!
        // #undef size_t
        // #ifdef _WIN64
        // __MINGW_EXTENSION typedef unsigned __int64 size_t;
        // #else
        // typedef unsigned int size_t;
        // #endif /* _WIN64 */

        // #define __size_t
        // #endif
        // #ifndef __SIZE_TYPE__
        // #define __SIZE_TYPE__ long unsigned int
        // #endif
        //注意这里，size_type 在不同平台下的定义不一样，所以使用auto可以完全匹配该平台下.size()的返回类型
    }

    return 0;
}