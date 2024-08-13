#include <iostream>


int main()
{
    // static_cast begin
    double pai{3.14};
    void* vp1 = &pai;

    // std::cout<<*vp1<<'\n'; //Error
    std::cout<<*static_cast<double*>(vp1)<<'\n'; //It's ok!
    std::cout<<*static_cast<int*>(vp1)<<'\n';  // undefined behavior

    const int* pi = new int{100};
    //std::cout<<*static_cast<int*>(pi)<<'\n'; //Error : static_cast can't cast away const

    //static_cast end

    //const_cast start
    const char* cpc = "just do it!";
    char* pc = const_cast<char*>(cpc);
    std::cout<<pc<<'\n';
    pc = "it do just";  // not recommand to do that
    std::cout<<pc<<'\n';

    //const_cast end

    //reinterpret_cast start
    int*p = new int{101};
    void* vp2 = p;
    char* q = reinterpret_cast<char*>(p);  // syntax right, but it's an undefined behavior
    int* rpi = reinterpret_cast<int*>(vp2); // okay!, Type matching!
    std::cout<<*rpi<<'\n';
    //reinterpret_cast end

    //dynamic cast start
    //to do
    //dynamic cast end
    return 0;
}