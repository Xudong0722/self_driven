#include <iostream>

class Test{
private:
    int data;
public:
    Test(int _data = 0) : data(_data){
        std::cout << "[Test]" << _data << " " << this << "\n";
    }

    Test(const Test& test){
        this->data = test.data;
        std::cout << "[Test const Test&] " << this << "\n";
    }

    Test(Test&& test) noexcept {
        this->data = std::move(test.data);
        std::cout << "[Test Test&&] " << this << "\n";
    }

    Test& operator=(const Test& test){
        this->data = test.data;
        std::cout << "[Test operator=(const Test&)] " << this << '\n';
    }

    Test& operator=(const Test&& test){
        this->data = std::move(test.data);
        std::cout << "[Test operator=(const Test&&)] " << this << '\n';
    }

    ~Test(){
        std::cout << "[~Test] " << this << "\n";
    }
};


int main()
{
    Test test;
    Test&& rref_test = std::move(test);

    Test obj1 = rref_test;
    Test obj2 = std::move(obj1);
    return 0;
}