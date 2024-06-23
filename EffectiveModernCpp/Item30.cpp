#include <iostream>
#include <string>

class Widget{
public:
    static const std::size_t Minvals = 28;
};

void f(std::size_t min_val){
    std::cout << min_val << '\n';
}

template<typename... T>
void perfect_forwarding(T&&... Args){
    f(std::forward<T>(Args)...);
}

const std::size_t Widget::Minvals;
int main()
{   
    /*
    Item30. 熟悉完美转发失败的情况
    1.花括号初始化
    2.0或者null作为空指针
    3.仅有声明的整形static const数据成员
    4.重载函数的名称和模板名称
    5.位域
     */


    //3
    f(Widget::Minvals);
    perfect_forwarding(Widget::Minvals);  //link error， 没有定义
    return 0;
}