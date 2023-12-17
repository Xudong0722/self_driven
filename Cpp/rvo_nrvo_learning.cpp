#include <iostream>

class Test{
private:
    int data;
public:
    Test(int _data = 0) : data(_data){
        std::cout << "[Test] " << this << "\n";
    }

    Test(const Test& test){
        this->data = test.data;
        std::cout << "[Test const Test&] " << this << "\n";
    }

    Test(Test&& test) noexcept {
        this->data = std::move(test.data);
        std::cout << "[Test Test&&] " << this << "\n";
    }

    ~Test(){
        std::cout << "[~Test] " << this << "\n";
    }
};

//NRVO: Named return value optimization
Test CreateTestDataWithNRVO(){
    Test test{100};
    return test;
}

//RVO:return value optimization
Test CreateTestDataWithRVO(){
    return Test{101};
}

int main()
{
    std::cout << "[CreateTestDataWithNRVO] begin\n";
    Test test_nrvo = CreateTestDataWithNRVO();
    std::cout << "[CreateTestDataWithNRVO] end\n";

    std::cout << "[CreateTestDataWithRVO] begin\n";
    Test test_rvo = CreateTestDataWithRVO();
    std::cout << "[CreateTestDataWithRVO] end\n";

    return 0;
}
