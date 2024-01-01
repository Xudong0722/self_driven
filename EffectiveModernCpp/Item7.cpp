#include <iostream>

class A{
public:
    A(int i, bool b){
        std::cout << "A(int, bool)\n";
    }
    A(bool b, int x){
        std::cout << "A(bool, int)\n";
    }
    A(std::initializer_list<double> ild){
        std::cout << "A(initializer_list)\n";
        for(const auto elem : ild){
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }
};

int main()
{
    {
        //三种初始化方式
        int x1 = 0;
        int x2(0);
        int x3{0};
    }

    //为什么尽量使用方式三-统一初始化方式？
    {
        //1.防止变窄转化
        long long x1{0};

        int x2(x1);  // ok
        //int x3{x1};  // narrowing conversion of 'x1' from 'long long int' to 'int' inside { } [-Wnarrowing]
    }

    {
        //2.C++最令人头疼的解析
        //A a();  // 这里其实是一个函数声明
        //A b{};  // Fine! 调用默认构造函数
    }

    // 有哪些缺点/坑？
    {
        //1.如果你定义的类的构造函数带有std::initializer_list的话，使用花括号初始化时编译器会想法设法的调用此构造函数

        A a(10, true);  // 调用A(int, bool)
        A b(true, 101); // 调用A(bool, int)
        A c{10, true};  // 调用A(initializer_list)
    }
    return 0;
}