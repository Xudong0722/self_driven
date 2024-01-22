#include "utility/XDLOG.h"
#include <iostream>

using namespace utility::elvis;

int main()
{
    XDLOG::GetInst().log(INFO, __FUNCTION__);

    int a{0};
    std::string b{"123"};
    double c{0.3};

    XDLOG::GetInst().log_test(a, b, c);
    return 0;
}