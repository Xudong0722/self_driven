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

class Data{
private:
    int* integer;
public:
    Data(){
        integer = new int{101};
        std::cout << "Construct at: " << this << '\n';
    }

    Data(const Data& data) noexcept{
        if(&data != this){
            std::cout << "Copy construct at " << this << " from " << &data << '\n';
            integer = new int{*data.integer};
        }
    }

    Data(Data&& data) noexcept{
        if(&data != this){
            std::cout << "Move construct at " << this << " from " << &data << '\n';
            integer = data.integer;
            data.integer = nullptr;
        }
    }

    ~Data(){
        delete integer;
        std::cout << "Destruct at " << this << '\n';
    }
};

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

        //Test t = CreateTestButUseMove(); //This function will prevents NRVO


        //关于什么情况下应该return std::move(x)：
        //1.x是一个左值，x如果是将亡值的话，是没必要的（RVO）
        //2.确保x之后不会再被使用！
        //3.x不应该是一个局部对象，Since C++11, return x (or return (x) or return ((x)) for that matter) will try to use the move constructor if x is a local variable or a function parameter.
        //现代C++编译器会有限考虑移动而非拷贝，并且尽可能消除副本，原地构造（moving a local object in a return statement prevents copy elision）
        //https://artificial-mind.net/blog/2021/10/23/return-moves
    }

    {
        std::cout << "------------------------------\n";
        Data data;
        
        std::vector<Data> vec(2, data);
        
        std::cout << "------------------------------\n";
        vec.push_back(data);  // 如果Data类的移动构造没有声明成noexcept的话，vector在扩容时不会调用移动构造，因为copy构造是异常安全的，编译器生成代码需要参考异常声明，非noexcept声明的移动构造会破坏push_back的异常安全保证
    }
    return 0;
}