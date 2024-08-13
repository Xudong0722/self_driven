#include <iostream>
#include <string>
#include <map>

struct item{
    bool flag{false};
    std::string s;
};

int main()
{
    std::map<std::string, item> mp;

    item elm{true, "123"};

    mp.emplace(elm.s, elm);

    std::cout << elm.s << std::endl;

    return 0;
}