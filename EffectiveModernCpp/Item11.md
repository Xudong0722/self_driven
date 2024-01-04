# Item-11. 优先考虑使用deleted函数而非使用未定义的私有声明

```c++
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


class friend_class{
public:
    void func(){
        a_class_not_suitable_for_copy_v1 a{1010};
        a.private_no_definition_func_v1(); 
    }
};
```

观察上面这两个类，a_class_not_suitable_for_copy_v1包含一个private且未定义的函数private_no_definition_func_v1，目的是不希望任何人调用它，包括友元
通过上面这种方法确实可以达到目的，但是编译时没问题的，在链接阶段会因为找不到实现而报错。

```c++
g++ -c Item11.cpp -o test.o -std=c++11
>

g++ test.o -o test -std=c++11
>test.o:Item11.cpp:(.text$_ZN12friend_class4funcEv[_ZN12friend_class4funcEv]+0x25): undefined reference to `a_class_not_suitable_for_copy_v1::private_no_definition_func_v1()'
collect2.exe: error: ld returned 1 exit status
```

如果我们将此函数声明为delete函数就会在编译阶段报错。

```c++
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
        a.private_no_definition_func_v1(); // 编译阶段没有问题， 在链接阶段会有问题
    }

    void func_v2(){
        a_class_not_suitable_for_copy_v2 b{2010};
        b.private_no_definition_func_v2(); // 编译阶段就会报错 
    }
};
```

```c++
g++ -c Item11.cpp -o test.o -std=c++11
>Item11.cpp: In member function 'void friend_class::func_v2()':
Item11.cpp:46:41: error: use of deleted function 'void a_class_not_suitable_for_copy_v2::private_no_definition_func_v2( '
         b.private_no_definition_func_v2();
                                         ^
Item11.cpp:30:10: note: declared here
     void private_no_definition_func_v2() = delete;
```

## 其他的原因

1. 通常，deleted函数被声明为public而不是private。这也是有原因的。当客户端代码试图调用成员函数，C++会在检查deleted状态前检查它的访问性。当客户端代码调用一个私有的deleted函数，一些编译器只会给出该函数是private的错误（译注：而没有诸如该函数被deleted修饰的错误），即使函数的访问性不影响它是否能被使用。所以值得牢记，如果要将老代码的“私有且未定义”函数替换为deleted函数时请一并修改它的访问性为public，这样可以让编译器产生更好的错误信息。
2. delete对所有函数都可以使用，可以禁用掉某些不合理的函数重载。
