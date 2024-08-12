#include <iostream>

class Object{
public:
    Object(){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    ~Object(){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    Object(const Object& obj){
        this->a = obj.a;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    Object(Object&& obj){
        this->a = obj.a;
        obj.a = 0;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    Object& operator=(const Object& obj){
        if(this == &obj){
            return *this;
        }

        this->a = obj.a;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return *this;
    }
    Object& operator=(Object && obj){
        if(this == &obj){
            return *this;
        }
        this->a = obj.a;
        obj.a = 0;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return *this;
    }
private:
    int a{0};
};

template<typename T>
void Swap(T& a, T& b) noexcept {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

class TestOnlyCopyCstructor{
public:
    TestOnlyCopyCstructor(const Object& obj): a(obj){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void test(){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

private:
    Object a;
};

class TestCopyAndMove{
public:
    TestCopyAndMove(const Object& obj): a(obj){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    
    TestCopyAndMove(Object&& obj): a(std::move(obj)){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void test(){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

private:
    Object a;
};

class TestOnlyValue{
public:
    TestOnlyValue(Object obj): a(std::move(obj)){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    

    void test(){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

private:
    Object a;
};

int main()
{
    {
        // Object a;
        // Object b;
        // Swap(a, b);
    }

    {
        //Only copy constructor
        //Object l_obj;
        //TestOnlyCopyCstructor t1(l_obj);   //copy once

        //TestOnlyCopyCstructor t2{Object()};   //copy once, Inefficient
    }

    {
        //copy and move
        //Object l_obj;
        //TestCopyAndMove t1(l_obj);   //copy once


        //TestCopyAndMove t2{Object()};  // move once, efficient
    }


    {
        //Pass value

        //Object l_obj;
        //TestOnlyValue t1(l_obj);  //copy once, move once

        //TestOnlyValue t2{Object()};  //move once
    }


    return 0;
}