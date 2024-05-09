/*
    ref: https://cntransgroup.github.io/EffectiveModernCppChinese/1.DeducingTypes/item1.html
    Item1: 理解模板类型推导
*/

#include <iostream>
#include <type_traits>

template<class T>
void funa(T& param){
    std::cout<<"funa begin\n";
    std::cout<<typeid(T).name() << std::endl;
    std::cout<< std::boolalpha
            << std::is_const<T>::value << std::endl;
    param = 10;
    std::cout<<"funa end\n";
}

// template<class T>
// void funb(T&& param);

template<class T>
void funb(T&& param){
    std::cout<<"funb begin\n";
    std::cout<<typeid(T).name()<<std::endl;
    std::cout<< std::boolalpha
            << std::is_const<T>::value << std::endl;
    param = 10;
    std::cout<<"funb end\n";
}

template<class T>
void func(T param);

// template<class T>
// void func(T param){
//     std::cout<<"func begin\n";
//     std::cout<<typeid(T).name()<<std::endl;
//     std::cout<< std::boolalpha
//             << std::is_const<T>::value << std::endl;
//     param = 10;
//     std::cout<<"func end\n";
// }

int main()
{
    int x = 100;
    const int cx = x;
    const int& rx = cx;

    // 测试一下三种方式下，const和reference信息是否被推导出来了
    func(x);
    //func(cx);
    //func(rx);
    //func(7);
    return 0;
}