#include <iostream>
#include <type_traits>

// template<class T>
// void print_x(T x){
//     std::cout<<typeid(T).name()<<'\n';
// }

template<class T>
T& modify(T& x){
    std::cout<<"x: "<<x<<'\n';
    x = 777;
    return x;
}

int& func(int& x){
    return x;
}

template<typename T> 
void f(T param){

}               

class Test{
    int u;
public:
    Test(int _u = 0): u(_u){}
};

int main()
{
    auto x = 7;   // x : int
    const auto cx = x; // cx : const int
    const auto& rx = cx; // rx : const int&

    
    auto&& uref1 = x;  // int&
    auto&& uref2 = cx; // const int&
    auto&& uref3 = 7;  // int&&
    auto&& uref4 = rx; // const int& 

    auto y = 17;
    uref1 = y; // ok
    //uref2 = y; // error, uref2 is read-only
    uref3 = y; //ok
    //uref4 = y; // error, uref4 is read-only

    auto var = modify<int>(y);
    std::cout<<"var: "<<var<<'\n';

    auto& tmp = func(y);
    std::cout << std::boolalpha << "Is tmp a reference? " << std::is_reference<decltype(tmp)>::value << std::endl;

    Test test;
    auto ix{test}; // Test
    auto iix = {test};  // std::initializer_list

    // 创建一个 std::initializer_list
    std::initializer_list<Test> initList = {test};

    // 使用 std::is_same 来检查类型是否是 std::initializer_list
    std::cout << "Is ix an initializer_list? " << std::is_same<decltype(ix), decltype(initList)>::value << std::endl;   //false
    std::cout << "Is iix an initializer_list? " << std::is_same<decltype(iix), decltype(initList)>::value << std::endl;  //true

    //f({ 11, 23, 9 });  // error: no matching function for call to 'f(<brace-enclosed initializer list>)'


    const char name[] = "elvis";
    const int ages[] = {1,2,3};
    auto another_name = name;   // 数组退化为指针-1
    auto another_ages = ages;  // 数组退化为指针-2

    auto another_func = func;  // 函数名退化为指针-3

    return 0;
}