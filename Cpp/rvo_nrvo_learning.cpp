#include <iostream>
//https://zhuanlan.zhihu.com/p/588852649
class Test{
private:
    int data;
public:
    Test(int _data = 0) : data(_data){
        std::cout << "[Test]" << _data << "\n";
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
Test CreateTestDataWithNRVO(bool condition){
    Test test{100};
    if(!condition){
        std::cout << "condition is " << condition << '\n';
        return Test{};
    }
    return std::move(test);   // https://stackoverflow.com/questions/14856344/when-should-stdmove-be-used-on-a-function-return-value
}

//RVO:return 
Test CreateTestDataWithRVO(){
    return Test{101};
}

int main()
{
    Test test_nrvo = CreateTestDataWithNRVO(true);

    Test test_rvo = CreateTestDataWithRVO();

    return 0;
}