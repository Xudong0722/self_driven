#include <iostream>
#include <atomic>
#include <vector>
#include <mutex>

class Polynomial{
public:
    using RootsType = std::vector<double>;

    RootsType Roots() const{   //返回多项式所有的根，因为这个操作不会修改Polynomial本身，但是会修改内部状态，所以声明成const是合理的

        //roots = ....执行了一个非常复杂的计算
        return roots;
    }

    RootsType RootsOnce() const{
        if(!RootsCached){
            //roots = ....          //step1
            RootsCached = true;     //step2
        }
        return roots;
    }

    RootsType RootsOnceThreadSafe1() const{
        std::lock_guard<std::mutex> lock(m);

        if(!RootsCached){
            //roots = ....          //step1
            RootsCached = true;     //step2
        }
        return roots;
    }

private:
    mutable std::mutex m;
    mutable RootsType roots{};   
    mutable bool RootsCached{false};
};

int main()
{
    //Item16.让const成员函数线程安全
    //1.确保const成员函数线程安全，除非你确定它们永远不会在并发上下文（concurrent context）中使用。
    //2.使用std::atomic变量可能比互斥量提供更好的性能，但是它只适合操作单个变量或内存位置。

    {
        //逻辑const
        //我们知道，类的成员函数在被调用的时候，会传入一个默认的参数-this指针，且这个指针top-level pointer(Pointer* const)
        //如果成员函数被声明成const成员函数，那么这个指针是top-low-level pointer(const Pointer* const)，即我们无法更改这个指针指向的对象的值
        //有一些情况会有些特殊
        
        //比如多项式类Polynomial，求根是很费时间的操作，我们希望要么不算，要么只算一次，所以我们可以用一个标记标识roots有没有计算过

        //问题是const成员函数对于外界来说应该是表达只读的操作，并不会修改类，所以应该是线程安全的，但是RootsOnce在被不同线程调用时可能会出现不同线程写同一块内存的情况，引发未定义行为

        //RootsOnceThreadSafe1利用互斥量解决了线程之间数据竞争的问题
    }
    return 0;
}