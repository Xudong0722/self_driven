# Memory management

## 1.使用动态内存

c++中不应该使用malloc/free，只应该使用new/delete(现代c++应该考虑使用智能指针)

区别1：
1.malloc/free不会调用构造函数和析构函数，只分配内存
2.new申请内存可能会抛出异常

```c++
#include <iostream>

class Foo
{
public:
	Foo(int x_ = 101) : x(x_) {
		std::cout << __FUNCTION__ << x << std::endl;
	}
	~Foo() {
		std::cout << __FUNCTION__ << x << std::endl;
	}
private:
	int x;
};

int main()
{
	Foo* my_foo{ (Foo*)malloc(sizeof(Foo)) };   //不会构造对象， 即不会调用构造函数
	Foo* my_other_foo{ new Foo(102) };  //不仅分配内存，还会构造对象

	free(my_foo);  //相对应的， 不会调用析构函数
	delete my_other_foo;  //会调用析构函数

	//不抛出异常的new，需要指定nothrow
	Foo* no_exc_foo{ new(std::nothrow) Foo(103) };
	delete no_exc_foo;
	return 0;
}
```

## 智能指针

1.标准智能指针shared_ptr, unique_ptr, weak_ptr可以简单直接的从函数按值返回
```c++
#include <iostream>
#include <memory>

class Foo
{
public:
	Foo(int x_ = 101) : x(x_) {
		std::cout << __FUNCTION__ << x << std::endl;
	}
	~Foo() {
		std::cout << __FUNCTION__ << x << std::endl;
	}
private:
	int x;
};

void close(FILE* file_ptr)
{
	if (nullptr == file_ptr) return;
	fclose(file_ptr);
	std::cout << "File closed.\n";
}


int main()
{
	FILE* f{ fopen("data.txt", "w") };
	std::shared_ptr<FILE> file_ptr{ f, close };   // shared_ptr 自定义deleter不需要指定模板类型参数

	std::unique_ptr<FILE, decltype(&close)> file_ptr2{ fopen("data2.txt", "w"), close }; //需要指定维模板类型参数
	return 0;
}
```

weakptr, 它可包含由shared_ptr管理的资源的引用，但是weak_ptr释放时不会影响shared_ptr
```c++
#include <iostream>
#include <memory>

class Simple
{
public:
	Simple() { m_int_ptr = new int{}; }
	~Simple() { delete m_int_ptr; }
	void setValue(int value) { *m_int_ptr = value; }
	int getValue() const { return *m_int_ptr; }
private:
	int* m_int_ptr;
};

void useRes(std::weak_ptr<Simple>& weak_simple) {
	auto res{ weak_simple.lock() };
	if (nullptr != res) {
		std::cout << "Resource still alive.\n";
	}
	else {
		std::cout << "Resource has been freed!\n";
	}
}

int main()
{
	auto shared_simple{ std::make_shared<Simple>() };
	std::weak_ptr<Simple> weak_simple{ shared_simple };

	useRes(weak_simple);

	shared_simple.reset();

	useRes(weak_simple);
	return 0;
}
```

enable_shared_from_this
安全的返回自身的shared_ptr或weak_ptr请继承enable_from_this<T>!

```c++
#include <iostream>
#include <memory>
#include <format>

class Foo : public std::enable_shared_from_this<Foo>
{
public:
	std::shared_ptr<Foo> getPointer() {
		return shared_from_this();
	}
	~Foo() {
		std::cout << __FUNCTION__ << std::endl;
	}

	void setValue(int val) { m_int = val; }
	int getValue() const { return m_int; }
private:
	int m_int{ 0 };
};

class RealFoo {
public:
	std::shared_ptr<RealFoo> getPointer() {
		return std::shared_ptr<RealFoo>(this);  //wrong imple, 这样会导致每个shared_ptr都有一个独立的控制块，会导致重复释放，引发异常
	}
	~RealFoo() {
		std::cout << __FUNCTION__ << std::endl;
	}
};

int main()
{
	auto ptr1{ std::make_shared<Foo>() };
	auto ptr2{ ptr1->getPointer() };

	ptr2->setValue(101);
	std::cout << ptr1->getValue() << std::endl;
	std::cout << std::format("{}, {}\n", (void*)ptr1.get(), (void*)ptr2.get());
	
	auto ptr3{ std::make_shared<RealFoo>() };
	auto ptr4{ ptr3->getPointer() };  //maybe crash when the object freed
	return 0;
}
```