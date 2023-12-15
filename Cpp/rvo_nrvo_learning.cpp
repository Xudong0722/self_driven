#include <iostream>
//https://zhuanlan.zhihu.com/p/588852649
class Test{
private:
    int data;
public:
    Test(int _data = 0) : data(_data){
        std::cout << "[Test]\n";
    }

    Test(const Test& test){
        this->data = test.data;
        std::cout << "[Test const Test&]\n";
    }

    Test(Test&& test) noexcept {
        this->data = std::move(test.data);
        std::cout << "[Test Test&&]\n";
    }

    ~Test(){
        std::cout << "[~Test]\n";
    }
};

//NRVO: Named return value optimization
Test CreateTestDataWithNRVO(){
    Test test{100};
    return test;
}

//RVO:return 
Test CreateTestDataWithRVO(){
    return Test{101};
}

int main()
{
    Test test_nrvo = CreateTestDataWithNRVO();

    Test test_rvo = CreateTestDataWithRVO();

    return 0;
}