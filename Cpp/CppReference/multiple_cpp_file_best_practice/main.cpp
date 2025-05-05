#include <iostream>
#include "Person.h"
#include "SingleObj.hpp"

int main()
{
    Person a{"12345", 25};
    
    std::cout << sizeof(a) << std::endl;

    SingleObj b{};

    std::cout << sizeof(b) << std::endl;
    return 0;
}