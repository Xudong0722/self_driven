```c++
#include <iostream>

class Test{
private:
    int data;
public:
    Test(int _data = 0) : data(_data){
        std::cout << "[Test]" << _data << " " << this << "\n";
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
        std::cout << "[Test operator=(const Test&)] " << this << '\n';
    }

    Test& operator=(const Test&& test){
        this->data = std::move(test.data);
        std::cout << "[Test operator=(const Test&&)] " << this << '\n';
    }

    ~Test(){
        std::cout << "[~Test] " << this << "\n";
    }
};


int main()
{
    Test test;
    Test&& rref_test = std::move(test);

    Test obj1 = rref_test;
    Test obj2 = std::move(obj1);
    return 0;
}
```

上述代码的输出是
```c++
[Test]0 0x61ff08
[Test const Test&] 0x61ff04
[Test Test&&] 0x61ff00
[~Test] 0x61ff00
[~Test] 0x61ff04
[~Test] 0x61ff08
```

从输出可以看出来，obj1调用的是拷贝构造，obj2调用的是移动构造，原因是std::move(obj1) 这里产生了一个右值，所以这里会调用移动构造，而对于`Test obj1 = rref_test;` rref_test是一个左值，所以调用的是拷贝构造，我们可以借助汇编代码验证：


```C
        push    rbp
        mov     rbp, rsp
        push    rbx
        sub     rsp, 40
        lea     rax, [rbp-28]
        mov     esi, 0
        mov     rdi, rax
        call    Test::Test(int) [complete object constructor]
        lea     rax, [rbp-28]
        mov     rdi, rax
        call    std::remove_reference<Test&>::type&& std::move<Test&>(Test&)
        mov     QWORD PTR [rbp-24], rax
        mov     rdx, QWORD PTR [rbp-24]
        lea     rax, [rbp-32]
        mov     rsi, rdx
        mov     rdi, rax
        call    Test::Test(Test const&) [complete object constructor]
        lea     rax, [rbp-32]
        mov     rdi, rax
        call    std::remove_reference<Test&>::type&& std::move<Test&>(Test&)
        mov     rdx, rax
        lea     rax, [rbp-36]
        mov     rsi, rdx
        mov     rdi, rax
        call    Test::Test(Test&&) [complete object constructor]
```

```c
        lea     rax, [rbp-28]
        mov     rdi, rax
        call    std::remove_reference<Test&>::type&& std::move<Test&>(Test&)
        mov     QWORD PTR [rbp-24], rax
        mov     rdx, QWORD PTR [rbp-24]
        lea     rax, [rbp-32]
```
这里可以看出来这里产生了一个临时的右值，所以我们调用的是移动构造函数。借此我们可以看出来，这里的右值，也即将亡值(xvalue)一般情况下是在表达式中产生，编译器为了执行完这条语句而创建的临时对象，对我们来说不可见。
