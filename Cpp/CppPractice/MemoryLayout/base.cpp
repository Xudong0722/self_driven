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

int main()
{
    BasePub a;
    BasePri b;
    return 0;
}