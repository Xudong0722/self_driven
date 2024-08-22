#include <iostream>
#include <type_traits>

template<typename T, typename T2 = typename std::enable_if< std::is_default_constructible<T>::value, int>::type>
void func()
{
    T t;
    T2 v{101};
    std::cout << "T is default constructible\n";
    t.print();
    std::cout << "v = " << v << std::endl;
}

class DefaultConstructible{
public:
    DefaultConstructible(int _a = 100): a(_a){

    }

    void print(){
        std::cout << "a = " << a << std::endl;
    }
private:
    int a;
};

class UnDefaultConstructible{
public:
    UnDefaultConstructible(int _a) : a(_a){

    }

    void print(){
        std::cout << "a = " << a << std::endl;
    }

private:
    int a;
};

int main()
{

    func<DefaultConstructible>();   //np

    //func<UnDefaultConstructible>();   //compile error, 当条件编译不满足时，模板实例化会失败
    return 0;
}