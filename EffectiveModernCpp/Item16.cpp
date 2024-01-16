#include <iostream>
#include <atomic>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>

class Polynomial {
public:
  using RootsType = std::vector<double>;

  RootsType Roots() const {   //返回多项式所有的根，因为这个操作不会修改Polynomial本身，但是会修改内部状态，所以声明成const是合理的

      //roots = ....执行了一个非常复杂的计算
    std::cout << "RootsType::Roots()\n";
    return roots;
  }

  RootsType RootsOnce() const {
    if (!RootsCached) {
      //roots = ....          //step1
      RootsCached = true;     //step2
    }
    std::cout << "RootsType::RootsOnce()\n";
    return roots;
  }

  RootsType RootsOnceThreadSafe1() const {
    std::lock_guard<std::mutex> lock(m);

    if (!RootsCached) {
      //roots = ....          //step1
      RootsCached = true;     //step2
    }
    std::cout << "RootsType::RootsOnceThreadSafe1()\n";
    return roots;
  }

  

private:
  mutable std::mutex m;
  mutable RootsType roots{};
  mutable bool RootsCached{ false };
};

void funa() {
  Polynomial p;
  p.RootsOnceThreadSafe1();
  return;
}

static int count = 0;

void cal(int n) {
  while (n--) {
    ++count;
  }
}

static std::atomic<int> ato_count = 0;

void cal_atomic_version(int n) {
  while (n--) {
    ++ato_count;
  }
}

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

    //RootsOnceThreadSafe1利用互斥量解决了线程之间数据竞争的问题，但是成本很高

    Polynomial p;
    std::thread j1{ funa };
    std::thread j2{ funa };
    j1.detach();
    j2.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  {
    //在操作单个变量或内存位置时，使用std::atomic变量可能更优
    int n = 1000000;
    std::thread j1{ cal, n };
    std::thread j2{ cal, n };
    std::cout << "Before: " << count << '\n';
    j1.join();
    j2.join();
    std::cout << "After: " << count << '\n';
    //Before: 0
    //After: 1023973   两个线程同事读写一个变量，导致count的值是不确定的

    //使用atomic可以解决多进程并发读写数据竞争的问题，同时效率高于互斥锁
    int nn = 1000000;
    std::thread j3{ cal_atomic_version, nn };
    std::thread j4{ cal_atomic_version, nn };
    std::cout << "Before: " << ato_count << '\n';
    j3.join();
    j4.join();
    std::cout << "After: " << ato_count << '\n';

    //Before: 0
    //After : 2000000
  }
  return 0;
}