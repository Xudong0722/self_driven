#include <memory>
#include <iostream>

class A{
public:
    A() {
        std::cout << __FUNCTION__ << std::endl;
    }
    ~A() {
        std::cout << __FUNCTION__ << std::endl;
    }
};

void funcA(std::shared_ptr<A> a){
    std::cout << __FUNCTION__ << std::endl;
    std::cout << a.use_count() << std::endl;
}

void funcB(const std::shared_ptr<A>& a){
    std::cout << __FUNCTION__ << std::endl;
    std::cout << a.use_count() << std::endl;
}

int main()
{
    std::cout << "Main start\n";
    std::shared_ptr<A> a = std::make_shared<A>();

    std::cout << a.use_count() << std::endl;

    std::cout << "Before call funcA\n";
    std::cout << a.use_count() << std::endl;
    funcA(a);
    std::cout << "After call funcA, ";
    std::cout << a.use_count() << std::endl;

    std::cout << "Before call funcB\n";
    std::cout << a.use_count() << std::endl;
    funcB(a);
    std::cout << "After call funcB, ";
    std::cout << a.use_count() << std::endl;

    std::cout << "Main end\n";

    return 0;
}