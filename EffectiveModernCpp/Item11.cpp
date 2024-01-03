#include <iostream>

class friend_class;
class a_class_not_suitable_for_copy_v1{
private:   
    // 通过将拷贝构造函数和拷贝赋值运算符设置为private并且只声明不实现达到目的2
    a_class_not_suitable_for_copy_v1(const a_class_not_suitable_for_copy_v1&);
    a_class_not_suitable_for_copy_v1& operator= (const a_class_not_suitable_for_copy_v1&);
    void private_no_definition_func_v1();

private:
    int m_data;
public:
    friend friend_class;
    a_class_not_suitable_for_copy_v1(int data): m_data(data){}
    void print(){
        std::cout << m_data << '\n';
    }
};

class a_class_not_suitable_for_copy_v2{
private:
    int m_data;
public:
    friend friend_class;
    a_class_not_suitable_for_copy_v2(int data): m_data(data){}
    // 通过将拷贝构造函数和拷贝赋值运算符 delete掉达到目的2
    a_class_not_suitable_for_copy_v2(const a_class_not_suitable_for_copy_v2&) = delete;
    a_class_not_suitable_for_copy_v2& operator= (const a_class_not_suitable_for_copy_v2&) = delete;
    void private_no_definition_func_v2() = delete;

    void print(){
        std::cout << m_data << '\n';
    }
};

class friend_class{
public:
    void func_v1(){
        a_class_not_suitable_for_copy_v1 a{1010};
        //a.private_no_definition_func_v1(); // 编译阶段没有问题， 在链接阶段会有问题
        a.print();
    }

    void func_v2(){
        a_class_not_suitable_for_copy_v2 b{2010};
        //b.private_no_definition_func_v2(); // 编译阶段就会报错 
        b.print();
    }
};

int main()
{
    //Item11. 优先考虑使用deleted函数而未定义的私有函数声明
    // 1.当我们不想类的某个成员被外界调用，我们可以将它的访问属性声明为private
    // 2.当我们不想类的某个成员函数被调用，我们可以只声明，不实现，这样子友元在调用时也会因为链接出问题

    // 针对第二点，有一个更好的办法就是此节描述的，考虑将其定义成delete函数

    {
        a_class_not_suitable_for_copy_v1 a(1010);
        a_class_not_suitable_for_copy_v1 b(1011);
        // a_class_not_suitable_for_copy_v1 c{b}; // error: 'a_class_not_suitable_for_copy_v1::a_class_not_suitable_for_copy_v1(const a_class_not_suitable_for_copy_v1&)' is private within this context
        // a = b; //error: 'a_class_not_suitable_for_copy_v1& a_class_not_suitable_for_copy_v1::operator=(const a_class_not_suitable_for_copy_v1&)' is private within this context
        a.print();
        b.print();
        friend_class d;
        d.func_v1();
    }

    {
        a_class_not_suitable_for_copy_v2 a(2010);
        a_class_not_suitable_for_copy_v2 b(2011);
        // a_class_not_suitable_for_copy_v2 c{b};  //error: use of deleted function 'a_class_not_suitable_for_copy_v2::a_class_not_suitable_for_copy_v2(const a_class_not_suitable_for_copy_v2&)' a_class_not_suitable_for_copy_v2 c{b};
        //a = b; //error: use of deleted function 'a_class_not_suitable_for_copy_v2& a_class_not_suitable_for_copy_v2::operator=(const a_class_not_suitable_for_copy_v2&)'
        a.print();
        b.print();
        friend_class d;
        d.func_v2();
    }

    return 0;
}