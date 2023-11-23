/*
    1. 加入删除器, 解决内存泄漏问题
    2. atomic 操作，有性能问题
    3. 优点：lazy load
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

    class deleter{
    public:
        ~deleter(){
            if(nullptr != singleton){
                delete singleton;
                singleton = nullptr;
            }
            std::cout<<"Singleton::delerer::~deleter()\n";
        }  
    };
public:
    static std::mutex singleton_mutex;
    static deleter m_deleter;
    static Singleton* GetSingletonInstance();
    void printAddress() const;
};

Singleton* Singleton::singleton = nullptr;
std::mutex Singleton::singleton_mutex;
Singleton::deleter Singleton::m_deleter;

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