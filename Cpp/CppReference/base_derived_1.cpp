#include <iostream>
#include <string>
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
    virtual bool func2(){
        cout << "Base::func2()\n";
        return false;
    }
    virtual void test2(){
        cout << "test2()\n";
        if(func2()){
            cout << "func2 return true\n";
        }else{
            cout << "func2 return false\n";
        }
    }

    int oreturn(){
        return 1;
    }
    void ofunc(){
        cout <<"Base ofunc" << oreturn() <<"\n";
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

    bool func2(){
        cout << "Drive::func2()\n";
        return true;
    }

    int oreturn(){
        return 3;
    }
private:
    int b{0};
};

int main()
{
    Drive* obj = new Drive();
    obj->ofunc();
    cout <<"---------------------------------------\n";
    //obj->Base::test2();
    std::string a;
    return 0;
}