/*
  1. C++11规定了local static在多线程条件下的初始化行为，要求编译器保证了内部静态变量的线程安全性。
*/

#include <iostream>
#include <mutex>

class Singleton{
private:
    Singleton(){ std::cout<<"Singleton()\n"; }
    ~Singleton(){ std::cout<<"~Singleton()\n"; }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:

    static Singleton& GetSingletonInstance();
    void printAddress() const;
};


Singleton& Singleton::GetSingletonInstance(){
    static Singleton singleton;
    return singleton;
}

void Singleton::printAddress() const{
    std::cout<<&GetSingletonInstance()<<std::endl;
}

int main()
{
    Singleton::GetSingletonInstance().printAddress();
    return 0;
}