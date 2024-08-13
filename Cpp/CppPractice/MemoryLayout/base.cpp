#include <iostream>
#include <string>

class BasePub{
public:
    int a;
    double b;
    int c;
    double d;
};

class BasePri{
    int a;
    std::string b;
    int c;
};

BasePub c;

int main()
{
    BasePub a;
    BasePri b;

    std::cout << &a << " " << &a.a << " " << &a.b << std::endl;
    std::cout << &b << std::endl;
    std::cout << &c << " " << &c.a << " " << &c.b << std::endl;
    
    return 0;
}