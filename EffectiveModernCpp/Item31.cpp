#include <iostream>
#include <vector>
#include <functional>

using FilterContainer =
std::vector<std::function<bool(int)> >;

FilterContainer filters;

void addFilter() 
{
    int divisor = 101;
    filters.emplace_back(
        [&](int value) -> bool {
            return value % divisor == 0;
        }
    );
}

class Widget {
public:
    Widget(int div = 1) : divisor(div) {}
    void addFilter() const {
        filters.emplace_back(
            [=](int value) -> bool {   //捕获只能应用于lambda创建时所在作用域里的non-static局部变量， 所以这里实际上捕获的是this
                return value % divisor == 0;
            }
        );
    }

    void addFilterCpp14() const {
        filters.emplace_back(
            [div = divisor](int value) -> bool {   
                return value % div == 0;
            }
        );
    }
private:
    int divisor;
};

int computeSomeValue1() {
    return 1;
}

int computeSomeValue2() {
    return 2;
}

int computeDivisor(int param1, int param2) {
    return param1 + param2;
}

void addDivisorFilter() {
    static int cal1 = computeSomeValue1();
    static int cal2 = computeSomeValue2();
    static int divisor = computeDivisor(cal1, cal2);

    filters.emplace_back(
        [=](int value) -> bool {   //使用默认按值捕获会让你认为lambda闭包是独立的，他已经将需要用到的参数都拷贝了一份，但事实并不如此
            return value % divisor == 0;  //这个lambda没有使用任何的non-static对象，所以没有捕获任何变量，然后缺引用了static变量divisor
        }
    );

    ++divisor;  //每次变动，上面添加的filter中的divisor也会变化，这可能并不是你期望的
}
int main()
{
    /*
    Item31. 避免使用默认捕获模式
     */

     //按引用捕获的风险点 - 可能使用悬空引用
    {
        //addFilter();

        int value = 202;
        for (const auto& filter : filters) {
            std::cout << "result: " << filter(value) << '\n';  //使用悬空引用，未定义行为
        }
    }

    Widget* w = new Widget(20);
    w->addFilter();
    w->addFilterCpp14();
    //delete w;  将导致line 56 调用filter时出现未定义行为

    for (const auto& filter : filters) {
        std::cout << "result: " << filter(100) << '\n';
    }


    delete w;  // It works.


    std::cout << "---------------------\n";
    addDivisorFilter();
    for (const auto& filter : filters) {
        std::cout << "result: " << filter(4) << '\n';
    }
    std::cout << "---------------------\n";
    addDivisorFilter();
    for (const auto& filter : filters) {
        std::cout << "result: " << filter(5) << '\n';
    }
    return 0;
}