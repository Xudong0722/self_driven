#include <iostream>
#include <memory>
#include <functional>

class Investment{
public:
    template<typename... Args>
    Investment(){}
    virtual ~Investment(){}
};

class Stock: public Investment{
public:
    template<typename... Args>
    Stock(){}
    ~Stock(){}
};

class RealEstate: public Investment{
public:
    template<typename... Args>
    RealEstate(){}
    ~RealEstate(){}
};

enum class INVESTMENT_TYPE{
    Stock = 1,
    RealEstate = 2,
};

template<typename... Args>
auto MakeInvestment(INVESTMENT_TYPE type, Args... args){
    auto deleteInvestment = [](Investment* investment){  //stateless function
        std::cout << "Delete investment " << &investment << '\n';
        delete investment;
    };

    std::unique_ptr<Investment, decltype(deleteInvestment)> investment{nullptr, deleteInvestment};
    if(type == INVESTMENT_TYPE::Stock){
        investment.reset(new Stock(std::forward<Args>(args)...));
    }
    if(type == INVESTMENT_TYPE::RealEstate){
        investment.reset(new RealEstate(std::forward<Args>(args)...));
    }

    return investment;
}

void print_args(){
    std::cout << '\n';
}

template<typename T, typename... Args>
void print_args(T first, Args... args){
    std::cout << first << ' ';
    print_args(args...);
}

void deleteInvestmentV2(Investment* investment){
    std::cout << "Delete investment v2: " << &investment << '\n';
    delete investment;
}

void print_num(int* a){
    std::cout << *a << '\n';
}

int main()
{
    //Item18.对于独占资源使用std::unique_ptr

    //1.std::unique_ptr是轻量级、快速的、只可移动（move-only）的管理专有所有权语义资源的智能指针
    //2.默认情况，资源销毁通过delete实现，但是支持自定义删除器。有状态的删除器和函数指针会增加std::unique_ptr对象的大小
    //3.将std::unique_ptr转化为std::shared_ptr非常简单

    print_args(1, 5.4, "123w132", 666);

    auto stock = MakeInvestment(INVESTMENT_TYPE::Stock);   
    std::cout << &stock << " " << sizeof(stock) << '\n';  //8, 无状态lambda表达式作为删除器

    using deleter = decltype(deleteInvestmentV2);   //C++ Primer p223： 当我们将decltype作用于某个函数时，它返回函数类型并非指针类型！
    deleter* a = deleteInvestmentV2;

    std::unique_ptr<Investment, deleter*> realestate{new RealEstate(), deleteInvestmentV2};  
    std::cout << &realestate << " " << sizeof(realestate) << '\n'; //16, 自定义函数作为删除器

    std::function<void(Investment*)> mydeleter(deleteInvestmentV2);
    std::unique_ptr<Investment, decltype(mydeleter)> realestate2{new RealEstate(), mydeleter};  
    std::cout << &realestate2 << " " << sizeof(realestate2) << '\n'; //40, 使用std::functon 更大啦！！！

    std::unique_ptr<int, decltype(print_num)*> psum{new int{101}, print_num};
    using sum_delete = decltype(print_num);
    const sum_delete* sum_deleter = psum.get_deleter();
    int* q = new int{114};
    (*sum_deleter)(q);
    return 0;
}