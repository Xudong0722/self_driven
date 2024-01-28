#include <iostream>
#include <memory>
#include <functional>

class Widget{
private:
    int data;
public:
    Widget(int data_): data(data_){
        std::cout << "Widget::Widget(int data)\n";
    }
    ~Widget(){
        std::cout << "Widget::~Widget()\n";
    }
};

int main()
{
    //Item19.对于共享资源使用std::shared_ptr
    //1.std::shared_ptr为有共享所有权的任意资源提供一种自动垃圾回收的便捷方式。
    //2.较之于std::unique_ptr，std::shared_ptr对象通常大两倍，控制块会产生开销，需要原子性的引用计数修改操作。
    //3.默认资源销毁是通过delete，但是也支持自定义删除器。删除器的类型是什么对于std::shared_ptr的类型没有影响。
    //4.避免从原始指针变量上创建std::shared_ptr。


    auto loggingDel = [](Widget* widget){
        std::cout << "widget " << &widget << " will be deleted.\n";
        delete widget;
    };
    {
        //shared_ptr创建的几种方式

        //1.使用make_shared来构造shared_ptr
        std::shared_ptr<Widget> sp1 = std::make_shared<Widget>(101);

        //2.从unique_ptr构造
        std::unique_ptr<Widget> up1 = std::make_unique<Widget>(102);
        std::shared_ptr<Widget> sp2 = std::move(up1);

        //3.从原始指针构造（不建议）
        std::shared_ptr<Widget> sp3{new Widget{103}};
            //先声明一个原始指针，然后从这个原始指针构造shared_ptr, 如果有多个会发生未定义行为
        auto p1 = new Widget{104};
        std::shared_ptr<Widget> sp4{p1, loggingDel};
        std::shared_ptr<Widget> sp5{p1, loggingDel};   // 从原始指针构造shared_pre将会新创一个一个control block(会包含引用计数),所以sp4和sp5指向一个对象但是却有两个不同的控制块，ref count归0时会重复delete
    }

    {
        //shared_ptr的大小
    }
    return 0;
}