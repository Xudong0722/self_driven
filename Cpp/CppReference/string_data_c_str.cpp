#include <iostream>
#include <string>

#define VAR(x) #x << ": " << x << " "

int main()
{
    std::string str;
    str.resize(10);

    std::cout << VAR(str) << VAR(str.size()) << VAR(str.capacity()) << std::endl;

    char t[] = "world\0";

    std::cout << VAR(t) << VAR(strlen(t)) << std::endl;

    // string::.data() can return char* after c++14
    memcpy(str.data(), t, strlen(t));

    std::cout << VAR(str) << VAR(str.size()) << VAR(str.capacity()) << std::endl;
    return 0;
}