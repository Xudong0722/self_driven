#include <memory>
#include <iostream>
#include <vector>

class A{
public:
    A() {
        std::cout << __FUNCTION__ << std::endl;
    }
    ~A() {
        std::cout << __FUNCTION__ << std::endl;
    }
    int getaa() const{
        return aa;
    }
private:
    int aa{101};
};

void funcA(std::shared_ptr<A> a){
    std::cout << __FUNCTION__ << std::endl;
    std::cout << a.use_count() << std::endl;
}

void funcB(const std::shared_ptr<A>& a){
    std::cout << __FUNCTION__ << std::endl;
    std::cout << a.use_count() << std::endl;
}


std::vector<std::shared_ptr<A>> vec;

class C{
public:
    C(const std::shared_ptr<A>& a)
     :pa(a){
        std::cout << __FUNCTION__ << std::endl;
    }
    ~C(){
        std::cout << __FUNCTION__ << " " << pa.use_count() << std::endl;
    }
    void print(){
        std::cout << __FUNCTION__ << " " << pa.use_count() << " " << pa->getaa() << std::endl;
    }
private:
    std::shared_ptr<A> pa;
};
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


    std::shared_ptr<A> b = std::make_shared<A>();

    vec.push_back(b);
    std::cout << b.use_count() << std::endl;
    C c(b);
    std::cout << b.use_count() << std::endl;
    vec.clear();
    std::cout << b.use_count() << std::endl;
    b.reset();
    c.print();
    return 0;
}