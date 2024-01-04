#include <iostream>
#include <memory>

class Base{
public:
    virtual bool f1() const{
        std::cout << "Base::bool f1() const\n";
        return true;
    }

    virtual Base* f2(){    //如果派生类要override此函数，返回类型和Base*是协变类型即可（covariant type），即is-a的关系
        std::cout << "Base::Base* f2()\n";
        return this;
    }

    virtual void f3(int x, bool b){
        std::cout << "Base::void f3(int, bool)\n";
    }

    virtual void f4(int x) &{
        std::cout << "Base::void f4(int)&\n";
    }
};

class DerivedWrongVersion: public Base{
public:
    virtual bool f1(){
        std::cout << "DerivedWrongVersion::bool f1()\n";
        return true;
    }

    virtual void f3(int x, bool b, double f){
        std::cout << "DerivedWrongVersion::void f3(int, bool, double)\n";
    }

    virtual void f4(int x) &&{
        std::cout << "DerivedWrongVersion::f4(int)&&\n";
    }
};

class DerivedRightVersion: public Base{
public:
    virtual bool f1() const override{
        std::cout << "DerivedRightVersion::bool f1() const\n";
        return true;
    }
    
    virtual DerivedRightVersion* f2() override{    //DerivedRightVersion是Base的子类，所以满足重写条件
        std::cout << "DerivedRightVersion::DerivedRightVersion* f2()\n";
        return this;
    }

    virtual void f3(int x, bool b) override{
        std::cout << "DerivedRightVersion::void f3(int, bool)\n";
    }

    virtual void f4(int x) & override{
        std::cout << "DerivedRightVersion::f4(int)&\n";
    }
};
int main()
{
    //Item12: 使用override声明重写函数
    //1. 编译器可以帮助你检查派生类和基类的virtual函数是否重写成功
    //2. 一目了然，此成员函数重写了基类的虚函数

    //Review
    /*
    重写一个虚函数需要满足:
        1.基类函数必须用virtual声明
        2.基类和派生类函数名必须要一样（析构函数除外）
        3.基类和派生类参数类型和数量必须要一样
        4.基类和派生类的函数常量性必须要一样，const限定
        5.基类和派生类的返回值和异常说明必须兼容
        6.基类和派生类的引用限定符必须完全一样（c++11）
    */ 
    
    {
        //错误的重写
        std::unique_ptr<Base> upb = std::make_unique<DerivedWrongVersion>();
        upb->f1();  //Base::bool f1() const   不符合条件4， const 限定不一致
        upb->f2();  //Base::Base* f2()
        upb->f3(101, true);  //Base::void f3(int, bool)  不符合条件3， 参数不一致
        upb->f4(3);  //Base::void f4(int)&  //不符合条件条件6
    }


    {
        //正确的重写
        std::unique_ptr<Base> upb = std::make_unique<DerivedRightVersion>();
        upb->f1();  //DerivedRightVersion::bool f1() const
        upb->f2();  //DerivedRightVersion::DerivedRightVersion* f2()
        upb->f3(101, true);  //DerivedRightVersion::void f3(int, bool)
        upb->f4(3);  //DerivedRightVersion::f4(int)&
    }
    return 0;
}