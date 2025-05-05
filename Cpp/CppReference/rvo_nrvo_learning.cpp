#include <iostream>
#include <vector>
class Test{
private:
    int data;
public:
    Test(int _data = 0) : data(_data){
        std::cout << "[Test]" << _data <<", addr: " << this << "\n";
    }

    Test(const Test& test){
        this->data = test.data;
        std::cout << "[Test const Test&] " << this << "\n";
    }

    Test(Test&& test) noexcept {
        this->data = std::move(test.data);
        std::cout << "[Test Test&&] " << this << "\n";
    }

    Test& operator=(const Test& test){
        this->data = test.data;
        std::cout << "[Test operator= const Test&] " << this << "\n";
        return *this;
    }

    
    Test& operator=(Test&& test) noexcept {
        this->data = std::move(test.data);
        std::cout << "[Test operator= Test&&] " << this << "\n";
        return *this;
    }

    int GetData() {
        return data;
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
        return Test{};  //Will prevent NRVO, because it is not the same object. if return test, it will be NRVO.
    }
    //return std::move(test);   // https://stackoverflow.com/questions/14856344/when-should-stdmove-be-used-on-a-function-return-value
    return test; 
}

//RVO:return value optimization
Test CreateTestDataWithRVO(){
    return Test{101};
}

Test CreateTestDateWithCondition(bool condition){
    Test test1{100};
    Test test2{101};
    return condition ? test1 : test2;  //Not NRVO, because it is not the same object.
}

Test CreateTestDateWithConditionNRVO(bool condition){
    Test test1{100};
    Test test2{101};
    
    if(condition){
        return test1;
    }
    return test2;
}

class Storage{
public:
    Storage(const std::vector<Test>& _data) : data(_data){
        std::cout << "[Storage] " << this << "\n";
    }

    ~Storage(){
        std::cout << "[~Storage] " << this << "\n";
    }

    const std::vector<Test>& GetData() const{
        return data;
    }
private:
    std::vector<Test> data;
};

Storage* pst{nullptr};

std::vector<Test> GetStroageData(){
    if(nullptr == pst){
        return {};
    }
    return pst->GetData();
}

void GetStroageDataV2(std::vector<Test>& data){
    if(nullptr == pst){
        return;
    }
    data = pst->GetData();
}

std::vector<Test> GetStroageDataV3(){
    std::vector<Test> data;
    std::cout << &data << "\n";
    data.emplace_back(Test{9999});
    return data;
}

int main()
{
    // std::cout << "[CreateTestDataWithNRVO] begin\n";
    // Test test_nrvo_ = CreateTestDataWithNRVO(true);
    // std::cout << "[CreateTestDataWithNRVO] end\n";

    // std::cout << "[CreateTestDataWithRVO] begin\n";
    // Test test_rvo = CreateTestDataWithRVO();
    // std::cout << "[CreateTestDataWithRVO] end\n";

    // Test test_non_return_obj = CreateTestDateWithCondition(false);

    //Test test_non_return_obj_nrvo = CreateTestDateWithConditionNRVO(false);

    pst = new Storage{std::vector<Test>{99, 999}};

    std::cout << "----------------------------\n";
    // auto vt = GetStroageData();
    //std::vector<Test> vt;
    //GetStroageDataV2(vt);
    auto vt3 = GetStroageDataV3();
    std::cout << &vt3 <<" " << vt3[0].GetData() << "\n";
    std::cout << "----------------------------\n";
    delete pst;
    return 0;
}
