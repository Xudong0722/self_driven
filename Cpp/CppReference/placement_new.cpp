#include <iostream>
#include <stdlib.h>

class Foo{
public:
    Foo() { std::cout << "Ctor\n"; }
    ~Foo() { std::cout << "Dtor\n"; }
};

int main() 
{
    void* buf = malloc(sizeof(Foo));
    Foo* p2 = new(buf)Foo();
    delete p2;
    return 0;
}