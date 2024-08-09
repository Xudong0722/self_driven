#include <iostream>

enum class Color{
    none = 0,
    red = 1,
    blue = 2,
};

void func(int u){
    std::cout << __FUNCTION__ << u << std::endl;
}

int main()
{
    int t {0};
    std::cin >> t;
    
    Color c = static_cast<Color>(t);

    switch(c){
        case Color::blue:{
            std::cout << "blue\n";
            break;
        }
        case Color::red:{
            std::cout << "red\n";
            break;
        }
        default:{
            std::cout << static_cast<std::underlying_type<Color>::type>(c) << std::endl;
        }
    }
    func(static_cast<int>(c));

    return 0;
}