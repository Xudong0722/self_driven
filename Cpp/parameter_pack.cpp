#include <iostream>

template<typename ...Args>
struct Tuple{};

template<int... args>
struct Array{
    static const int size = sizeof...(args);
    int array[size] = {args...};
};
int main()
{
    //Tuple 
    Tuple<> t0;
    Tuple<int> t1;
    Tuple<double, int> t2;
    //Tuple<0> t3;  0 isn't type

    //Array
    Array<1,2,3,4> a;
    for(int i = 0; i<a.size; ++i){
        std::cout << a.arrar[i] << ' ';
    }
    return 0;
}