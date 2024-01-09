#include <iostream>
#include <vector>

int throw_exception_func_but_noexp(int numerator, int denominator) noexcept {
    //return the value of denominator/numerator, throw exception when the numerator is zero
    int result = denominator/numerator;
    std::cout << "result: " << result << '\n';
    return result;
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

    Data(Data&& data) {  //should be noexcept
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
    //Item14.cpp
    //1.noexcept是函数接口的一部分，这意味着调用者可能会依赖它
    //2.noexcept函数较之于non-noexcept函数更容易优化
    //3.noexcept对于移动语义，swap，内存释放函数和析构函数非常有用
    //4.大多数函数是异常中立的（译注：可能抛也可能不抛异常）而不是noexcept

    {
        //将函数声明成noexcept，编译器会尽可能优化，如果noexcept函数抛出了异常，程序会直接调用std::terminate
        int denominator{0}, numerator{0};
        std::cin >> denominator >> numerator;
        std::cout << throw_exception_func_but_noexp(numerator, denominator) << '\n';
    }

    {
        //对于移动构造，noexcept很有用，标准库中比如vector的push_back, emplace_back，如果元素的移动构造不是noexcept的话，vector扩容时不会选择移动构造

        Data data;
        std::vector<Data> vec(2, data);
        vec.push_back(data);

        /*
        result:
        Construct at: 0x61fdf8
        Copy construct at 0x1a1690 from 0x61fdf8
        Copy construct at 0x1a1698 from 0x61fdf8
        Copy construct at 0x1a1700 from 0x61fdf8
        Copy construct at 0x1a16f0 from 0x1a1690
        Copy construct at 0x1a16f8 from 0x1a1698
        */

        //从以上结果来看，调用push_back时，由于容量已达上限，所以vector扩容，由于我们Data类的移动构造没有声明为noexcept，vector并没有执行move，而是copy
        //Effective modern c++这样解释：
        //当新元素添加到std::vector，std::vector可能没地方放它，换句话说，std::vector的大小（size）等于它的容量（capacity）。
        //这时候，std::vector会分配一个新的更大块的内存用于存放其中元素，然后将元素从老内存区移动到新内存区，然后析构老内存区里的对象。
        //在C++98中，移动是通过复制老内存区的每一个元素到新内存区完成的，然后老内存区的每个元素发生析构。这种方法使得push_back可以提供很强的异常安全保证：如果在复制元素期间抛出异常，std::vector状态保持不变，因为老内存元素析构必须建立在它们已经成功复制到新内存的前提下。
        //在C++11中，一个很自然的优化就是将上述复制操作替换为移动操作。但是很不幸运，这会破坏push_back的异常安全保证。
        //如果n个元素已经从老内存移动到了新内存区，但异常在移动第n+1个元素时抛出，那么push_back操作就不能完成。但是原始的std::vector已经被修改：有n个元素已经移动走了。恢复std::vector至原始状态也不太可能，因为从新内存移动到老内存本身又可能引发异常。
    }


    {
        //对于swap函数来说，noexcept也很有用
    }
    return 0;
}