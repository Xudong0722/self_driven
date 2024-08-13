#include <iostream>

struct BaseFunc{
    int a;
    double b;
    
    void func(){
        std::cout << "a = " << a << " b = " << b << std::endl;
    }
};

int main()
{


    return 0;
}