#include <iostream>

template<typename ...Args>
struct Tuple{};

template<int... args>
struct Array{
    static const int size = sizeof...(args);
    int array[size] = {args...};
    void outputArray(){
        std::cout << size << '\n';
        for(int x : array){
            std::cout << x << '\n';
        }
    }
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
    a.outputArray();
    return 0;
}