#include <bitset>
#include <iostream>
#include <map>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <set>

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

class BloomFilter {
public:
    BloomFilter(size_t sz, size_t hash_cnt)
        : hash_count(hash_cnt), size(sz) {
        bs.resize(sz);
    }

    void add(const std::string& str) {
        for(size_t i = 0; i < hash_count; ++i) {
            size_t index = hash_index(str, i) % size;
            bs[index] = true;
        }
    }

    bool contains(const std::string& str) {
        for(size_t i = 0; i < hash_count; ++i) {
            if(!bs[hash_index(str, i) % size]) {
                return false;
            }
        }
        return true;
    }

private:
    size_t hash_index(const std::string& str, size_t index) {
        std::hash<std::string> hash_fn;
        return hash_fn(str + std::to_string(index));
    }
private:
    std::vector<bool> bs;
    size_t hash_count;
    size_t size;
    
};

std::unordered_map<std::string, bool> hash_map;
int main() {
    BloomFilter bf(1000, 5);

    std::set<std::string> s;

    for(int i = 0; i < 1000000; ++i) {
        std::string str;
        for(int j = 0; j < 50; ++j) {
            str += rand() % 26 + 'a';
        }
        s.insert(str);
    }

    for(const auto& str : s) {
        bf.add(str);
        hash_map[str] = true;
    }

    // std::cout << "Bloom Filter size: " << sizeof(bf) << std::endl;
    // std::cout << "Hash Map size: " << (sizeof(void*) * 3 + 50) * s.size() << std::endl;

    //check the rightness of bloom filter
    for(const auto& str : s) {
        bool bf_res = bf.contains(str);
        bool hash_map_res = hash_map[str];
        if(bf_res != hash_map_res) {
            std::cout << "Error: " << str << " not match!" << std::endl;
            return 0;
        }
    }
    std::cout << "All strings are matched!" << std::endl;
    return 0;
}