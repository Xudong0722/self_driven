#include <iostream>
#include <memory>
#include <vector>

class Widget{
    //...
};

int computePriority(){
    int priority{0};
    //....  maybe throw a exception here
    return priority;
}
void processWidget(std::shared_ptr<Widget> spw, int priority){
    //...
}

int main()
{
    //Item21.优先考虑使用std::make_unique和std::make_shared，而非直接使用new


    //理由1.高效
    /*
    make_unique和make_shared支持任意多的参数，支持完美转发
    并且智能指针需要记录一些额外信息，比如引用计数，这些存放在智能指针的控制块中，也是需要申请内存的。
    如果我们使用make_unique/make_shared只需要申请一次内存就可以，如果使用new，再将new的结果传递给智能指针的构造函数
    则需要申请两次内存
    */


    //理由2.编写异常安全的代码
    processWidget(std::shared_ptr<Widget>(new Widget), computePriority());
    /*
    上面这个调用就是一个非异常安全的代码调用。
    函数在调用前会将所有的参数都计算完成，所以对于上面这个函数，在调用前有三个事情必须要完成：
     a. 调用new 在对堆上为widget对象申请内存
     b. 调用shared_ptr的构造函数
     c. 调用computePriority()函数
    但是编译器可能不是按照上面的顺序生成的代码，如果a先执行，然后c执行，过程中抛出了异常，那么a申请的内存就会泄漏
    */

    processWidget(std::make_shared<Widget>(), computePriority());  //异常安全的调用！


    // 一个提醒，本条款旨在阐述make的好处，并建议倾向于使用make，但并不是所有情况下make都适用
    /*
    a. 比如某些类重载了new和delete，那么初衷应该是期望精细化控制该类的内存，这种情况下使用make并不合适
    b. make不支持指定删除器
    c. 类期望使用花括号初始化
    */

    auto uva = std::make_unique<std::vector<int>>(10,20);
    auto vb = {10, 20};
    auto uvb = std::make_unique<std::vector<int>>(vb);   //如果期望使用花括号初始化可以使用这种方式

    for(int i = 0; i < (*uva).size(); ++i){
        std::cout << (*uva)[i] << ' ';  //20 20 20 20 20 20 20 20 20 20 并不是 
    }
    std::cout <<'\n';
    for(int i = 0; i<(*uvb).size(); ++i){
        std::cout << (*uvb)[i] << ' ';  //10 20
    }
    /*
        对于std::shared_ptrs，其他不建议使用make函数的情况包括(1)有自定义内存管理的类；(2)特别关注内存的系统，非常大的对象，以及std::weak_ptrs比对应的std::shared_ptrs活得更久。
    */
    return 0;
}