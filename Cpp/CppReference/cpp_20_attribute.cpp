#include <iostream>
#include <chrono>

double ans{1.0};

void another_func(){
    ans *= 1;
}

void func_with_attribute(bool condition){
    if(condition) [[likely]] {
        ans *= 0.54839578943789;
    }else [[unlikely]]{
        another_func();
    }
}

void func_without_attribute(bool condition){
    if(condition)  {
        ans *= 0.54354365564445;
    }else {
        another_func();
    }
}

namespace BENCHMARK {
	static std::chrono::time_point<std::chrono::steady_clock> start{};
	static std::chrono::time_point<std::chrono::steady_clock> end{};
	void BENCHMARK_BEGIN() {
		start = std::chrono::steady_clock::now();
	}

	void BENCHMARK_END() {
		end = std::chrono::steady_clock::now();
	}

	void OUTPUT_DURATION() {
		auto duration = std::chrono::duration<double, std::milli>(end - start).count();
		std::cout << duration << "ms" << std::endl;
	}
}

int main()
{
    int a{0};
    std::cin >> a;
    BENCHMARK::BENCHMARK_BEGIN();
    for(int i = 0; i < 1000000; i++){
        func_with_attribute(a);
    }
    BENCHMARK::BENCHMARK_END();
    BENCHMARK::OUTPUT_DURATION();

    std::cout << ans << std::endl;
    ans = 1.0;

    BENCHMARK::BENCHMARK_BEGIN();
    for(int i = 0; i < 1000000; i++){
        func_without_attribute(a);
    }
    BENCHMARK::BENCHMARK_END();
    BENCHMARK::OUTPUT_DURATION();
    std::cout << ans << std::endl;
    return 0;
}