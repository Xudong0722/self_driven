#include <iostream>

using namespace std;

class Base{
public:
    Base(){
        std::cout << "Base::Base()\n";
    }
    virtual ~Base(){
        cout << "Base::~Base()\n";
    }

    virtual void test(){
        cout << "Base::test()\n";
    }
    virtual bool func(){
        cout << "Base::func()\n";
        return false;
    }
private:
    int a{0};
};

class Drive: public Base{
public:
    Drive(){
        cout << "Drive::Drive()\n";
    }
    ~Drive(){
        cout << "Drive::~Drive()\n";
    }
    void test(){
        if(func()){
            cout <<"func return true\n";
        }else{
            cout <<"func return false\n";
        }
    }

    bool func(){
        cout << "Drive::func()\n";
        return true;
    }
private:
    int b{0};
};

int main()
{
    Base* obj = new Drive();
    obj->test();
    cout <<"---------------------------------------\n";
    obj->Base::test();
    return 0;
}