#include <map>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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

using namespace std;

constexpr const int N = 10000000;
constexpr const int M = 1000000;

template<typename T, typename V>
void bench_mark(const T& t, const V& v){
    const auto cnt = v.size();
    BENCHMARK::BENCHMARK_BEGIN();
    for(int i = 0; i<cnt; ++i){
        t.find(i);
    }
    BENCHMARK::BENCHMARK_END();
    BENCHMARK::OUTPUT_DURATION();
}
int main()
{
    srand(time(0));
    //map<int,int> mp;
    unordered_map<int, int> ump;
    
    for(int i = 0; i<N; ++i){
       //mp[i] = i;
       ump[i] = i;
    }

    vector<int> idx(M, 0);
    for(int i = 0; i<M; ++i){
        idx[i] = i;
    }

    //bench_mark(mp, idx);
    bench_mark(ump, idx);

    cout << "----------------------------\n";

    for(int i = 0; i<M; ++i){
        idx[i] = rand() % M;
    }

    //bench_mark(mp, idx);
    bench_mark(ump, idx);

    return 0;
}
