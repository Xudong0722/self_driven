# chapter2.string 与 string_view

```c++
#include <iostream>
#include <optional>
#include <format>
#include <string>
#include <vector>

int main()
{
	//不需要转义符号
	const char* ptr{ R"(hello"world"!)" };
	std::cout << ptr << '\n';

	//三向比较运算符来比较字符串
	std::string a{ "12" };
	std::string b{ "34" };

	auto result{ a <=> b };
	if (std::is_lt(result)) std::cout << "less" << std::endl;
	if (std::is_gt(result)) std::cout << "greater" << std::endl;
	if (std::is_eq(result)) std::cout << "equal" << std::endl;

	//兼容C风格的字符串
	std::string comp{ "1234" };
	const char* c_style_str_1 = comp.c_str();
	char* c_style_str_2 = comp.data();
	c_style_str_2[0] = '9';
	std::cout << std::format("1:{}, 2:{}", c_style_str_1, c_style_str_2) << std::endl;

	//CTAD(类模板参数推导)
	using namespace std::literals;//标准用户定义字面量在std::literals::string_literals中定义
	std::vector arr_str{ "john", "mike", "joe" };  //std::vector<const char*>
	std::vector arr_str_ex{ "john"s, "Mike"s, "Zoe"s };

	//字符串<->整数
	long double d{ 3.14l };
	std::string sd{ std::to_string(d) };
	std::cout << std::format("d to string is {}", sd) << std::endl;

	const std::string toParse{ "  1234dxz" }; 
	size_t index{ 0 };
	int value{ std::stoi(toParse, &index, 10) };
	std::cout << std::format("Parse value: {}", value) << std::endl;
	std::cout << std::format("First non-parsed character: '{}'", toParse[index]) << std::endl;

	//性能较好的转换函数
	double original_num{ 0.432154356 };
	const size_t buffer_size{ 10 };
	std::string out(20, ' ');
	auto [res1, err1] { std::to_chars(out.data(), out.data() + out.size(), original_num)};
	if (err1 == std::errc{}) std::cout << out << std::endl;

	double last_num{ 0.0 };
	auto [res2, err2] {std::from_chars(out.data(), out.data() + out.size(), last_num)};
	if (err2 == std::errc{}) {
		if (original_num == last_num) {
			std::cout << "Perfect roundtrip" << std::endl;
		}
		else {
			std::cout << "No perfect roundtrip" << std::endl;
		}
	}

	//string_view
	std::string ss{ "hello" };
	//std::string_view ssv{ ss + "world" };  //dangerous, 不要使用string_view保存临时字符串的视图, ub
	//std::cout << ssv << std::endl;  

	//字符串格式化
	int first{ 101 }, second{ 202 };
	std::string exc{ std::format("first{}, second{}", first, second) };
	std::cout << exc << '\n';
	std::cout << std::format("first{1}, second{0}", second, first) << '\n';

	int i{ 42 };
	std::cout << std::format("|{:5}|", i) << std::endl;  //指定宽度
	std::cout << std::format("|{:<5}|", i) << std::endl;  //左对齐
	std::cout << std::format("|{:>5}|", i) << std::endl;  //右对齐
	std::cout << std::format("|{:^5}|", i) << std::endl;  //居中
	std::cout << std::format("|{:_^10}|", i) << std::endl; // 指定默认填充字符
	return 0;
}
```