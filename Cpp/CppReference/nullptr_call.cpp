#include <iostream>

class ITest{
public:
    virtual void hello(){
        std::cout << "ITest::hello()" << std::endl;
    }
    virtual ~ITest() = default;
};

class Test: public ITest{
public:
    Test() { std::cout << "Test()" << std::endl; }
    ~Test() { std::cout << "~Test()" << std::endl; }
    virtual void hello() { std::cout << "hello()" << std::endl; }
};

class Test2{
public:
    Test2() { std::cout << "Test2()" << std::endl; }
    ~Test2() { std::cout << "~Test2()" << std::endl; }
    void hello() { std::cout << "hello() a = " << a << std::endl; }
private:
    int a{0};
};

int main()
{
    // Test* t = nullptr;  //call success, but this is an undefined behavior
    Test2* t = nullptr;
    t->hello();  //use Test2::a, will dereference this.
    return 0;
}