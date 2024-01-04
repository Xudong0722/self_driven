#include <iostream>
#include <utility>
#include <vector>
#include <string>
 
struct Test{
    std::string title;
    std::string descripation;

    friend std::ostream& operator<<(std::ostream& output, const Test& test){
        output << "[Test] " << test.title << " " << test.descripation << '\n';
        return output;
    }
};

Test CreateTest(){
    Test t;
    //...
    return t;
}

Test CreateTestButUseMove(){
    Test t;
    //...
    return std::move(t); //bad
}

int main()
{
    std::string str = "Hello";
    std::vector<std::string> v;
 
    // 使用 push_back(const T&) 重载，
    // 表示我们将带来复制 str 的成本
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
 
    // 使用右值引用 push_back(T&&) 重载，
    // 表示不复制字符串；而是
    // str 的内容被移动进 vector
    // 这个开销比较低，但也意味着 str 现在可能为空。
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
 
    std::cout << "The contents of the vector are \"" << v[0] << "\", \"" << v[1] << "\"\n";
    

    Test test;
    test.title = "title";
    test.descripation = "descripation";
    std::cout << test;
    
    Test obj;
    obj = std::move(test);
    std::cout << test << obj;


    {
        // Don't overuse std::move
        Test t = CreateTest();  //This function will trigger NRVO

        Test t = CreateTestButUseMove(); //This function will prevents NRVO


        //关于什么情况下应该return std::move(x)：
        //1.x是一个左值，x如果是将亡值的话，是没必要的（RVO）
        //2.确保x之后不会再被使用！
        //3.x不应该是一个局部对象，Since C++11, return x (or return (x) or return ((x)) for that matter) will try to use the move constructor if x is a local variable or a function parameter.
        //现代C++编译器会有限考虑移动而非拷贝，并且尽可能消除副本，原地构造
        //https://artificial-mind.net/blog/2021/10/23/return-moves
    }
    return 0;
}