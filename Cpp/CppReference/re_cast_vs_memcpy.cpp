#include <iostream>
#include <cstdint>
#include <bit>

int main()
{
    float f = 3.1415926f;
    //会触发UB
    std::cout << "Original float: " << f << std::endl;
    uint32_t i = *reinterpret_cast<uint32_t*>(&f);
    std::cout << "Reinterpreted as uint32_t: " << i << std::endl;
    float f2 = *reinterpret_cast<float*>(&i);
    std::cout << "Reinterpreted back to float: " << f2 << std::endl;
    
    std::cout << "------------------------\n";
    //跨平台，标准
    uint32_t tmp{0};
    memcpy(&tmp, &f, sizeof(f));
    std::cout << "Original float: " << f << std::endl;
    float f3{0};
    memcpy(&f3, &tmp, sizeof(tmp));
    std::cout << "memcpy to float: " << f3 << std::endl;
    
    std::cout << "------------------------\n";
    //c++20 bit_cast
    uint32_t tmp2 = std::bit_cast<uint32_t>(f);
    std::cout << "Original float: " << f << std::endl;
    std::cout << "bit_cast to uint32_t: " << tmp2 << std::endl;
    float f4 = std::bit_cast<float>(tmp2);
    std::cout << "bit_cast to float: " << f4 << std::endl;
    return 0;
}