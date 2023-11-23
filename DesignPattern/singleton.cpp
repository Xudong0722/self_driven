/*
    1. 没有删除器，存在内存泄漏
    2. 利用了互斥锁，比较耗费资源
    3. 在较老的编译器中可能并不是线程安全的，如果发生指令reorder的话
*/


#include <iostream>
#include <mutex>

class Singleton{
private:
    static Singleton* singleton;
    Singleton(){ std::cout<<"Singleton()\n"; }
    ~Singleton(){ std::cout<<"~Singleton()\n"; }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static std::mutex singleton_mutex;
    static Singleton* GetSingletonInstance();
    void printAddress() const;
};

Singleton* Singleton::singleton = nullptr;
std::mutex Singleton::singleton_mutex;

Singleton* Singleton::GetSingletonInstance(){
    if(nullptr == singleton){
        std::lock_guard<std::mutex> lock(singleton_mutex);
        if(nullptr == singleton){
            singleton = new Singleton();
        }
    }
    return singleton;
}

void Singleton::printAddress() const{
    std::cout<<singleton<<std::endl;
}

int main()
{
    // Singleton a;
    // a.printAddress();

    // Singleton b;
    // b.printAddress();

    // a.GetSingletonInstance()->printAddress();
    // b.GetSingletonInstance()->printAddress();

    Singleton::GetSingletonInstance()->printAddress();
    return 0;
}