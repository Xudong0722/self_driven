#include <memory>
#include <iostream>
#include <vector>

struct IA
{
    virtual ~IA(){};
};

struct A: public IA
{
    A(int a) { std::cout << "A\n";}
    ~A() { std::cout << "~A\n"; }
};

std::vector<std::unique_ptr<IA>> g_ias;

// IA* func()
// {
//     auto tmp = std::make_unique<A>(100);
//     g_ias.emplace_back(tmp);
//     return tmp.get();
// }

IA* fund()
{
    auto tmp = std::make_unique<A>(100);
    g_ias.emplace_back(std::move(tmp));
    return tmp.get();
}

IA* fune()
{
    auto tmp = std::make_unique<A>(100);
    auto ret = tmp.get();
    g_ias.emplace_back(std::move(tmp));
    return ret;
}



int main()
{   
    //std::cout << func() << std::endl;
    std::cout << fund() << std::endl;
    std::cout << fune() << std::endl;

    g_ias.clear();
    std::cout.flush();
    
    return 0;
}