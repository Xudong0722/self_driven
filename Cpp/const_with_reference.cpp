#include <iostream>
#include <string>

class Test{

public:
    template<class T>
    auto test_func(T x) -> decltype(x){
        return x;
    }
};

int func(){
    return 3;
}
int main()
{
    Test test;
    int u{110};
    std::string v{"elvis"};

    std::cout<<test.test_func<int>(u)<<'\n';
    std::cout<<test.test_func<std::string>(v)<<'\n';

    //int& ref1 = func();  //非常量引用的初始值必须为左值
    const int& ref2 = func();  //It's ok! 理解： reference to const & pointer to const仅仅是引用和指针的自以为是，他们认为自己指向了常量，所以自觉地不去修改他的值
    const int* p1 = &u; // It's ok! 但是u并不是一个常量，
    //--------------------------------
    int& ref1 = u;  // 引用本身不是一个对象，C++不允许随意更改引用所绑定的对象
    int x{111};
    ref1 = x;

    std::cout<<u<<" "<<x<<" "<<ref1<<'\n';
    ref1 = 100;
    std::cout<<u<<" "<<x<<" "<<ref1<<'\n';
    //---------------------------------

    //top-level const: can't change itself
    //loc-level const: the related object can't be changed
    int var{120};
    const int cv = var;  // top-level const
    const int* p2 = &cv; // low-level const
    const int* const p3 = p2; // the first const is low-level const, the second const is top-level const

    // when we execute copy operation, we need to notice that the object and the copied object have same low level const qualifications
    p2 = p3; // It's ok!
    //int* p4 = p2; // Error, p2 has low level const attribute but p4 not
    //int* const p5 = p2; // Error, p2 has low level const attribute but p5 only has top level const attribute

    std::cout<<var<<" "<<cv<<" "<<*p2<<" "<<*p3<<'\n';
    return 0;
}