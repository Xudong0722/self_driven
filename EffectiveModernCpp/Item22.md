# Item 22当使用Pimpl惯用法，请在实现文件中定义特殊成员函数

看下面一个例子：

Gadget.h
```c++
#pragma once
#include <vector>
#include <string>
#include <iostream>

class Gadget
{
public:
	Gadget() {}
	~Gadget() {}
	void outputInfo();
private:
	std::vector<int> vec;
	std::string info;
};
```

Gadget.cpp
```c++
#include "Gadget.h"

void Gadget::outputInfo()
{
	std::cout << info << '\n';
}
```

Widget.h
```c++
#pragma once
#include <memory>

class Gadget;

class Widget{
public:
	Widget();
	//~Widget();
private:
	std::unique_ptr<Gadget> p_impl;
};
```

Widget.cpp
```c++
#include "Widget.h"
#include "Gadget.h"

Widget::Widget() :
	p_impl(std::make_unique<Gadget>()) {

}
```

main.cpp
```c++
#include "Widget.h"

int main() {
	Widget w;
	return 0;
}
```

Widget是我们定义的一个对象，它拥有一个成员-智能指针p_impl，我们在Widget.h文件中没有给出Gadget的完整定义，仅仅使用了前向声明，然后Widget类中声明了一个指向这个对象的unique_ptr， 这是允许的，但是这个代码没有办法通过编译。报错如下：

```c++
Severity	Code	Description	Project	File	Line	Suppression State
Error	C2338	static_assert failed: 'can't delete an incomplete type'
```

原因是Widget没有显示声明析构函数了，所以编译器会为它生成一个默认的析构函数，对于成员p_impl，也会使用默认删除器，原因就在这里：
```c++
_EXPORT_STD template <class _Ty>
struct default_delete { // default deleter for unique_ptr
    constexpr default_delete() noexcept = default;

    template <class _Ty2, enable_if_t<is_convertible_v<_Ty2*, _Ty*>, int> = 0>
    _CONSTEXPR23 default_delete(const default_delete<_Ty2>&) noexcept {}

    _CONSTEXPR23 void operator()(_Ty* _Ptr) const noexcept /* strengthened */ { // delete a pointer
        static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
        delete _Ptr;
    }
};
```
默认删除器使用了static_assert，用到了sizeof运算符，在生成代码的时候，编译器还没有看见Gadget的定义，它是一个不完整类型，sizeof运算符是不能作用于大小都未知的类型的，所以我们遇到了这个报错。解决的办法就是显示声明析构函数，并将实现放在Gadget定义之后。

附上不完整类型的wiki：
`C/C++中不完全类型有三种不同形式： void、未指定长度的数组以及具有非指定内容的结构和联合。使用不完全类型的指针或引用，不需要知道类型的全部内容。另外也用于解决对象类型及其成员之间的类型循环依赖。`



