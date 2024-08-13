#include <iostream>
#include <string>

class Shape{
public:
    Shape(){
        std::cout << __FUNCTION__ << std::endl;
    }
    virtual ~Shape(){
        std::cout << __FUNCTION__ << std::endl;
    }
};


class Circle: public Shape{
public:
    Circle(){
        std::cout << __FUNCTION__ << std::endl;
    }
    ~Circle(){
        std::cout << __FUNCTION__ << std::endl;
    }
};

class Triangle : public Shape{
public:
    Triangle(){
        std::cout << __FUNCTION__ << std::endl;
    }
    ~Triangle(){
        std::cout << __FUNCTION__ << std::endl;
    }
};


class result{
public:
    result(){
        std::cout << __FUNCTION__ << std::endl;
    }
    ~result(){
        std::cout << __FUNCTION__ << std::endl;
    }
};


result func(const Shape& a, const Shape& b){
    std::cout << __FUNCTION__ << std::endl;
    return result();
}

int main()
{
    //result&& r = func(Circle(), Triangle());
    /*
    Shape
    Triangle
    Shape
    Circle
    func
    result
    ~Circle
    ~Shape
    ~Triangle
    ~Shape
    ~result   延迟析构， 纯右值的生命周期和右值引用绑定
    */

    result&& r = std::move(func(Triangle(), Circle()));
    /*
    Shape
    Circle
    Shape
    Triangle
    func
    result
    ~result  //move是一个函数，函数调用结束对象就被析构了，r将会变成一个悬空引用
    ~Triangle
    ~Shape
    ~Circle
    ~Shape  
    */
    
    return 0;
}