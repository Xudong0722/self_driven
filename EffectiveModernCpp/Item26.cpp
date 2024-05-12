#include <iostream>
#include <set>
#include <chrono>
#include <ctime>
#include <iomanip>

std::multiset<std::string> names;
void logAndAdd(const std::string& name){
    std::cout << "logAndAdd(const std::string&)\n";
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm* ptm = std::localtime(&tt);
    std::cout << "current time is: " << std::put_time(ptm, "%c") << std::endl;

    names.insert(name);
    std::cout << "-----------------\n";
}

template<typename T>
void logAndAdd(T&& t){
    std::cout << "logAndAdd(T&& t)\n";
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm* ptm = std::localtime(&tt);
    std::cout << "current time is: " << std::put_time(ptm, "%c") << std::endl;

    names.insert(std::forward<T>(t));
    std::cout << "-------------------\n";
}

std::string nameFromIndex(int idx){
    //....
    std::string result{"john"};
    return result;
}

class Person{
public:
    template<typename T>
    Person(T&& t)
    : name(std::forward<T>(t)){
        std::cout << "name(T&& t)\n";
    }

    Person(int idx)
    : name(nameFromIndex(idx)){
        std::cout << "Person(int)\n";
    }
private:
    std::string name;
};

int main()
{
    /*
    Item26:避免在通用引用上重载
    */

    //我们看这三个调用, 无一例外，三者都会拷贝到names当中
    std::string str{"east"};
    logAndAdd(str);  //lvalue , 这种没有办法，只能拷贝
    logAndAdd(std::string("temp")); //rvalue， 可以优化一下，用移动而非拷贝
    logAndAdd("tmp");  //literal ， 最好是移动都不要有，直接原地在names中构造

    //形参为通用引用是贪婪的，他可以精确匹配大部分实参，所以重载通用引用往往会发生一些问题
    //我们看Person类有两种构造函数
    Person p1("elvis");  //ok
    Person p2(str);      //ok
    Person p3(100);      //ok
    size_t idx{101};
    Person p4(idx);      //Error!  因为size_t 首先匹配到通用引用的版本，精确匹配优先于类型转换的版本，所以尝试传递一个整数给string的构造函数引发了错误
    return 0;
}