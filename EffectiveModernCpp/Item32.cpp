#include <iostream>
#include <memory>
#include <functional>

class Widget {
public:
    int GetValue() const { return val; }
    void SetValue(int val_) { val = val_; }
private:
    int val{ 101 };
};

class FuncC11 {
public:
    explicit FuncC11(std::unique_ptr<Widget>&& ptr) : pw(std::move(ptr)) {}
    int operator()() const {
        if (nullptr == pw) {
            std::cout << "pw is nullptr\n";
            return -1;
        }
        return pw->GetValue();
     }
private:
    std::unique_ptr<Widget> pw;
};
int main()
{
    /*
    Item32: 使用初始化捕获来移动对象到闭包中

    C++14支持初始化捕获，而C++11不支持，我们来看看如何在c++11如何实现
     */


     //C++14
    std::unique_ptr<Widget> pw = std::make_unique<Widget>();
    pw->SetValue(102);

    auto func_c14 = [pw = std::move(pw)] {   //左侧的pw作用域是闭包内， 右侧pw作用域和闭包类所在作用域一致，这是c++14的实现方式
        std::cout << pw->GetValue() << '\n';
    };

    auto func_c14_v2 = [pw = std::make_unique<Widget>()] {
        std::cout << pw->GetValue() << '\n';
    };


    std::unique_ptr<Widget> pw1 = std::make_unique<Widget>();
    //C++11, verison 1
    auto res = FuncC11(std::make_unique<Widget>())();
    std::cout << res << '\n';

    //C++11, version 2
    auto func_c11 = std::bind([](const std::unique_ptr<Widget>& pw) {
        std::cout << pw->GetValue() << '\n';
        }, std::move(pw1));

    func_c14();
    func_c11();
    func_c14_v2();

    return 0;
}