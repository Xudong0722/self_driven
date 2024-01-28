#include <iostream>
#include <string>
#include <cstring>

#define oneToNine "123456789"

int main()
{
    char* a = "1234";
    std::cout << strlen(a) << '\n';

    std::string d{"123456789\0"};
    std::cout << d << " " << d.size() << " " << d.length() << " " << d.capacity() << '\n';

    d += '\0';
    std::cout << d << " " << d.size() << " " << d.length() << " " << d.capacity() << '\n';
    if(d == oneToNine){
        std::cout << "d == 123456789\n";
    }else{
        std::cout << "d != 123456789\n";
    }

    const char* dd = d.c_str();
    std::cout << dd << '\n';
    if(0 == strcmp(dd, oneToNine)){
        std::cout << "d == 123456789\n";
    }else{
        std::cout << "d != 123456789\n";
    }

    char* ccc = new char[strlen(a)+1];
    strcpy(ccc, a);
    ccc[4] = '1';
    std::cout << ccc << '\n';
    std::cout << strlen(ccc) << '\n';
    std::string cccc{ccc};
    std::cout << cccc << " " << cccc.size() << " " << cccc.length() << " " << cccc.capacity() << '\n';
    if(cccc == "1234"){
        std::cout << "cccc == 1234\n";
    }else{
        std::cout << "cccc != 1234\n";
    }
    return 0;
}