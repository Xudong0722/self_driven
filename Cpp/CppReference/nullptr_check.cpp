#include <iostream>
#include <string>
#include <vector>

template<typename T>
class U{
public:
    U(const T& t = T()): m(t){
        std::cout << "U Construct " << m << '\n';
    }
private:
    T m;
};


unsigned int mx = 10;

int main()
{
    int* p = new int(10);

    std::cout << p << '\n';
    std::cout << static_cast<void*>(p) << '\n';

    U<char> u;

    int* q = new int[mx];
    std::vector<int> vec(10, 1);
    size_t idx{0};
    for(int x : vec){
        q[idx++] = x;
    }

    for(int i = 0; i<idx; ++i){
        std::cout << q[i] << ' ';
    }
    return 0;
}