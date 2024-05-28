# chapter1.c++和标准库速成

## 1.头文件
c++ 20 支持import导入头文件

`note: C++20 ‘import’ only available with ‘-fmodules-ts’, which is not yet enabled with ‘-std=c++20’`

## 2.std::optional

```c++
#include <iostream>
#include <optional>

std::optional<int> getValue(bool assert) {
	if (assert) {
		return 101;
	}
	return std::nullopt;
}

int main()
{
	//optional
	std::optional<int> a{ getValue(true)};
	std::optional<int> b{ getValue(false) };

	if (a.has_value()) {
		std::cout << a.value() << '\n';
	}

	std::cout << b.value_or(1010) << '\n';
	
	return 0;
}
```

## 3.引用
引用本身就是顶层const，无法二次绑定。所以引用变量必须总是在创建时被初始化

```c++
#include <iostream>
#include <optional>
#include <format>

int main()
{
	int a{ 101 }, b{ 102 };
	int& const ra = a;
	ra = b;
	std::cout << ra << " " << a << '\n';  //102 102
	a = 201;
	std::cout << ra << " " << a << '\n';  //201 201 

	const int& rb = b;
	rb = 1010;   //error , low-level const, we can not modify it.
	return 0;
}
```

## 4.结构化绑定
必须为结构化绑定使用auto关键字


```c++
#include <iostream>
#include <optional>
#include <format>

int main()
{
	struct Point {
		double m_x;
		double m_y;
	};


	Point point;
	point.m_x = 1.1, point.m_y = 1.2;

	auto [x, y] { point };
	std::cout << std::format("x = {}, y = {}", x, y);  // x = 1.1, y = 1.2
	return 0;
}
```