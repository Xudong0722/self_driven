#include <iostream>

class A{
public:
    A(int _x) : x(_x){

    }
    ~A() = default;

    static void Test();
private:
    void func_test();
    int x;
};

void A::func_test(){
    std::cout << __FUNCTION__ << " " << x << std::endl;
}

static A* sa = nullptr;

void A::Test(){
    if(nullptr != sa){
        std::cout << sa->x << std::endl;
    }
    sa->func_test();
}

int main()
{
    sa = new A(100);
    A::Test();
    //std::cout << sa->x << std::endl;  //error

    
    return 0;
}