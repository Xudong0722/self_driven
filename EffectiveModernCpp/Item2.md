# 理解auto类型推导

实际上，auto类型推导和模板类型推导非常类似，这里不再赘述。

主要提一下C++的花括号初始化-uniformed initialization

```c++
class Test{
    int u;
public:
    Test(int _u = 0): u(_u){}
};

    Test test;
    auto ix{test}; // Test - 1
    auto iix = {test};  // std::initializer_list - 2

    // 创建一个 std::initializer_list
    std::initializer_list<Test> initList = {test};

    // 使用 std::is_same 来检查类型是否是 std::initializer_list
    std::cout << "Is ix an initializer_list? " << std::is_same<decltype(ix), decltype(initList)>::value << std::endl;   //false
    std::cout << "Is iix an initializer_list? " << std::is_same<decltype(iix), decltype(initList)>::value << std::endl;  //true

```

从以上代码可以看出来， 如果使用花括号初始化的话，使用auto推导，方式一下会推导成Test类型，但是在方式二下会推导成initializer_list类型，经过验证，确实如此。

另外，使用auto推导也会有函数名退化为函数指针，数组退化为数组指针的现象

```c++

    const char name[] = "elvis";
    const int ages[] = {1,2,3};
    auto another_name = name;   // 数组退化为指针-1
    auto another_ages = ages;  // 数组退化为指针-2

    auto another_func = func;  // 函数名退化为指针-3
```