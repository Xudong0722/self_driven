#include <iostream>

class Test{
private:
    int data;
public:
    Test(int _data = 0) : data(_data){
        std::cout << "[Test]" << _data << "\n";
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
Test CreateTestDataWithNRVO(bool condition){
    Test test{100};
    if(!condition){
        std::cout << "condition is " << condition << '\n';
        return Test{};
    }
    return std::move(test);   // https://stackoverflow.com/questions/14856344/when-should-stdmove-be-used-on-a-function-return-value
    //return test; 
}

//RVO:return value optimization
Test CreateTestDataWithRVO(){
    return Test{101};
}

int main()
{
    Test test_nrvo = CreateTestDataWithNRVO(true);
    std::cout << "[CreateTestDataWithNRVO] begin\n";
    Test test_nrvo_ = CreateTestDataWithNRVO(true);
    std::cout << "[CreateTestDataWithNRVO] end\n";

    std::cout << "[CreateTestDataWithRVO] begin\n";
    Test test_rvo = CreateTestDataWithRVO();
    std::cout << "[CreateTestDataWithRVO] end\n";

    return 0;
}
