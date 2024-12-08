#include <iostream>
#include <vector>

int main()
{
    std::vector<int> a;
    std::cout << a.size() << " " << a.capacity() << std::endl;
    return 0;
}

// g++ -Wall debug_or_build.cpp -o test
// nm ./test.out grep size|c++filt

// g++ -Wall -O2 debug_or_build.cpp -o test
// nm ./test.out grep size|c++filt