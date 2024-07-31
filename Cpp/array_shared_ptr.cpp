#include <array>
#include <vector>
#include <memory>
#include <iostream>

class Foo{
public:
    Foo(int value = 0) : m_value(value){
        std::cout << "Foo(" << m_value << ") constructed." << std::endl;
    } 
    ~Foo(){
        std::cout << "Foo(" << m_value << ") destructed." << std::endl;
    }
    int getValue() const{
        return m_value;
    }
private:
    int m_value;
};

int main()
{
    //array

    std::array<std::shared_ptr<Foo>, 3> arr;
    arr[0] = std::make_shared<Foo>(1);
    arr[1] = std::make_shared<Foo>(2);
    arr[2] = std::make_shared<Foo>(3);

    //auto tmp = arr[1];
    std::cout << "Before setting nullptr to arr:\n";
    for(const auto& item : arr){
        if(nullptr == item) continue;
         std::cout << "Foo value: " << item->getValue() << std::endl;
    }

    arr[1] = nullptr;
    std::cout << "After setting nullptr to arr:\n";
        for(const auto& item : arr){
        if(nullptr == item) continue;
         std::cout << "Foo value: " << item->getValue() << std::endl;
    }
    return 0;
}