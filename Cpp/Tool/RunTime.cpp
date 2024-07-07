
#include <iostream>
#include <chrono>


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
