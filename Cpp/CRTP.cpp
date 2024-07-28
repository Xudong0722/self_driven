#include <cmath>
#include <iostream>


template<class Derived>
class Shape{
public:
    void area() {
        static_cast<Derived*>(this)->computeArea();
    }
};

class Circle : public Shape<Circle>{
public:
    Circle(double _radius = 0.0) : radius(_radius){}

    void computeArea() const{
        double area = radius * radius * 3.1415926;
        std::cout << __FUNCTION__ << " " << area << std::endl;
    }
private:
    double radius{2.0};
};


class Rectangle : public Shape<Rectangle>{
public:
    Rectangle(double _width = 0.0, double _height = 0.0) : width(_width), height(_height){}

    void computeArea() const{
        auto area = width * height;
        std::cout << __FUNCTION__ << " " << area << std::endl;
    }

private:
    double width{3};
    double height{4};
};


//1.利用CRTP实现静态多态
void example_1(){
    Circle circle{5.0};
    circle.area();

    Rectangle rectangle{3.0, 4.0};
    rectangle.area();
}


//2.利用CRTP实现技术类，统计对象构造/析构的次数

template<class T>
struct counter{
    static int obj_created;
    static int obj_alive;

    counter(){
        ++ obj_alive;
        ++obj_created;
    }

    counter(const counter&){
        ++ obj_alive;
        ++ obj_created;
    }
protected:
    ~counter(){
        -- obj_alive;
    }
};

template<class T>
int counter<T>::obj_created = 0;

template<class T>
int counter<T>::obj_alive = 0;


class X: public counter<X>{

};


class Y: public counter<Y>{

};

void example_2(){
    X x1;
    {
        X x2;
    }
    
    {
        Y y1;
        Y y2;
    }


    std::cout << X ::obj_created << " " <<  X:: obj_alive << std::endl;
    std::cout << Y ::obj_created << " " <<  Y:: obj_alive << std::endl;
}
int main()
{
    example_1();

    example_2();
    return 0;
}


