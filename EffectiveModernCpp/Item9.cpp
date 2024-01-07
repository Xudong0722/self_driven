#include <iostream>
#include <type_traits>
#include <vector>
#include <list>

namespace elvis{
    //typename 用于告诉编译器此标识符是一个类型
    template<typename _Tp>
    struct remove_reference
    { typedef _Tp   type; };

    template<typename _Tp>
    struct remove_reference<_Tp&>
    { typedef _Tp   type; };

    template<typename _Tp>
    struct remove_reference<_Tp&&>
    { typedef _Tp   type; };

    /// Alias template for remove_reference
    template<typename _Tp>
    using remove_reference_t = typename remove_reference<_Tp>::type;

    /// Alias template for add_lvalue_reference
    // template<typename _Tp>
    // using add_lvalue_reference_t = typename add_lvalue_reference<_Tp>::type;

    /// Alias template for add_rvalue_reference
    // template<typename _Tp>
    // using add_rvalue_reference_t = typename add_rvalue_reference<_Tp>::type;
}

template<typename Tp>
class DealWithData{
    //typedef Tp DataType;
    //typedef DataType* DataPointer;
    //这里使用别名依赖Tp，因为模板的类型不确定，所以需要typename告诉编译器这是一个类型
    //typedef typename Tp::const_iterator CIT;

    using DataType = Tp;
    using DataPointer = DataType*;
    using CIT = typename Tp::const_iterator;
private:
    Tp container;
public:
    DealWithData() noexcept = default;
    DealWithData(const Tp& x){
        container = x;
    }
    DealWithData(Tp&& x){
        container = x;
    }

    void print(){
        std::cout << "Print Data: ";
        for(CIT cit = container.cbegin(); cit != container.cend(); ++cit){
            std::cout << *cit << ' ';
        }
        std::cout << '\n';
    }
};

class Widget{
};

template<typename T>                            //MyAllocList<T>是
using MyAllocList = std::list<T, MyAlloc<T>>;   //std::list<T, MyAlloc<T>>
                                                //的同义词

MyAllocList<Widget> lw;

template<typename T> 
class MyAllocList{
    // typedef std::list<T, MyAlloc<T>> type;
};

int main()
{
    {
        // 别名
        typedef int Integer; // OK
        using FloatNum = float; // OK

        Integer x{101};
        FloatNum y{0.3f};
    }

    
    {
        int x{101};
        int& rx = x;
        elvis::remove_reference<decltype(rx)>::type xx{102};  //Since C++11
        elvis::remove_reference_t<decltype(rx)> xxx{103};  //Since C++14
        //Same as remove_const and add_lvalue_reference
    }

    {
        std::vector<int> x{1,2,3,4,5};
        DealWithData<std::vector<int>> obj(std::move(x));
        obj.print();
    }
    return 0;
}