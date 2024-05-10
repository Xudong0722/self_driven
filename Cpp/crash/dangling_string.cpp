#include <iostream>
#include <string>
#include <map>

struct Info {
    int id{ 0 };
    std::string name;
    double grade{ 0.0 };
};

void func(int id, const Info& info) {
    std::map<int, const char*> id_name;

    id_name.insert(std::make_pair(id, info.name.c_str()));

    for (const auto& item : id_name) {
        std::cout << item.first << " " << item.second << '\n';
    }
}

std::string foo() {
    return std::string{ "foo" };
}

std::string create_string() {
    const char* str = foo().c_str();   // dangling pointer, 因为foo()调用完之后，返回的临时变量就会被释放掉，这样str就成了一个悬空指针，使用它会发生未定义行为
    return std::string(str);
}

std::string create_string_v2(){
    std::string copy = foo();
    const char* str = copy.c_str();
    return std::string(str);
}

int main()
{
    Info info;
    info.name = create_string();
    std::cout << info.name << '\n';  //maybe crash or output nothing

    info.name = create_string_v2();
    std::cout << info.name << '\n'; //foo
    func(100, info);
    return 0;
}