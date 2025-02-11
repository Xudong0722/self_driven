#include <cstdio>
#include <random>
#include <thread>
#include <cstdint>
#include <cstring>
#include <mutex>

constexpr uint32_t RecordCount = 100000000;
constexpr uint32_t NameLength = 10;
constexpr uint32_t StationCount = 200;
char file_buffer[RecordCount * (NameLength + 8)];

struct SimpleHashTable {
    constexpr static uint32_t HashTableSize = 1024;
    alignas(256) struct Slot {
        uint32_t hash {0};
        uint32_t minv {0};
        uint32_t maxv {0};
        uint32_t sumv {0};
    } slots[HashTableSize];
    void initialize () {
        memset(slots, 0, sizeof slots);
        for(auto & slot : slots)
            slot.minv = 500;
    }
    inline Slot & Get (const char * & ptr) {
        uint32_t raw_hash = 0;
        for(; *ptr != ':'; ptr++) {
            raw_hash = ((raw_hash<<3) + *ptr) ^ (raw_hash >> 5);
        }
        uint32_t slot = raw_hash & (HashTableSize - 1);
        while(slots[slot].hash != raw_hash && slots[slot].hash) [[unlikely]] {
            slot++;
            if(slot >= HashTableSize) [[unlikely]] slot = 0;
        }
        slots[slot].hash = raw_hash;
        return slots[slot];
    }
};

thread_local SimpleHashTable hash_table;

SimpleHashTable global_hash_table;
std::mutex global_hash_table_mutex;

char names[StationCount][NameLength];
int main () {
    // Generate data.
    std::mt19937 random_engine(123);
    for(auto i = 0u; i<StationCount; i++) {
        auto RealNameLength = NameLength - 2 + random_engine() % 3;
        for(auto j = 0u; j<RealNameLength; j++) {
            bool upper_case = random_engine() % 2;
            names[i][j] = 'a' + random_engine() % 26 + (upper_case ? 'A' - 'a' : 0);
        }
    }
    auto ptr = file_buffer;
    for(auto i = 0u; i<RecordCount; i++) {
        uint32_t name_index = random_engine() % StationCount;
        for(auto j = 0u; j<NameLength; j++) {
            if(names[name_index][j] == 0) break;
            *ptr++ = names[name_index][j];
        }
        *ptr++ = ':';
        if(random_engine() % 100 < 75) *ptr++ = '0' + random_engine() % 10;
        *ptr++ = '0' + random_engine() % 10;
        *ptr++ = '.';
        *ptr++ = '0' + random_engine() % 10;
        *ptr++ = '\n';
    }
    uint32_t file_buffer_len = ptr - file_buffer;

	global_hash_table.initialize();
    // Program start
    auto tp_start = std::chrono::steady_clock::now();

    uint32_t num_threads = std::thread::hardware_concurrency();
    std::thread threads[num_threads + 1];
    auto thread_func = [&] (uint32_t thread_id) {
        hash_table.initialize();
        auto start = file_buffer_len / num_threads * thread_id;
        auto end = file_buffer_len / num_threads * (thread_id + 1);
        if(start && file_buffer[start-1] != '\n') while(file_buffer[start++] != '\n') ;
        for(const char * ptr = file_buffer + start; ptr < file_buffer + end; ) {
            auto & slot = hash_table.Get(ptr);
            auto chr0 = *(ptr+1) - '0';
            auto chr1 = *(ptr+2) - '0';
            auto chr2 = *(ptr+3) - '0';
            auto chr3 = *(ptr+4) - '0';
            uint32_t temp{0}, offs{0};
            if(chr1 == '.'-'0') [[unlikely]] {
                temp = chr0 * 10 + chr2;
                offs = 4;
            } else {
                temp = chr0 * 100 + chr1 * 10 + chr3;
                offs = 5;
            }
            slot.minv = std::min(slot.minv, temp);
            slot.maxv = std::max(slot.maxv, temp);
            slot.sumv += temp;
			//printf("%d, %d, %d, %d\n", chr0,chr1,chr2,chr3);	
			//printf("%u, %u, %u\n", slot.minv, slot.maxv, slot.sumv);
            ptr += offs;
        }

		{
			std::unique_lock<std::mutex> lock(global_hash_table_mutex);
			for(auto i = 0u; i<SimpleHashTable::HashTableSize; i++) {
				global_hash_table.slots[i].minv = std::min(global_hash_table.slots[i].minv, hash_table.slots[i].minv);
				global_hash_table.slots[i].maxv = std::max(global_hash_table.slots[i].maxv, hash_table.slots[i].maxv);
				global_hash_table.slots[i].sumv += hash_table.slots[i].sumv;
				global_hash_table.slots[i].hash = hash_table.slots[i].hash;
				//printf("%u %u %u %u\n", hash_table.slots[i].hash, hash_table.slots[i].minv, hash_table.slots[i].maxv, hash_table.slots[i].sumv);
			}
		}
    };
    for(auto i = 0u; i<num_threads; i++) {
        threads[i] = std::thread(thread_func, i);
    }
    std::this_thread::yield();
    for(auto i = 0u; i<num_threads; i++) {
        threads[i].join();
    }

    // for(auto i = 0u; i<SimpleHashTable::HashTableSize; i++) {
    //     global_hash_table.slots[i].minv = std::min(global_hash_table.slots[i].minv, hash_table.slots[i].minv);
    //     global_hash_table.slots[i].maxv = std::max(global_hash_table.slots[i].maxv, hash_table.slots[i].maxv);
    //     global_hash_table.slots[i].sumv += hash_table.slots[i].sumv;
	// 	// global_hash_table.slots[i].hash = hash_table.slots[i].hash;
	// 	printf("%u %u %u %u\n", hash_table.slots[i].hash, hash_table.slots[i].minv, hash_table.slots[i].maxv, hash_table.slots[i].sumv);
    // }

    // Program ends
    auto tp_end = std::chrono::steady_clock::now();
    printf("Time: %lldms\n", std::chrono::duration_cast<std::chrono::milliseconds>(tp_end - tp_start).count());

   for(auto i = 0u; i<SimpleHashTable::HashTableSize; i++) {
		if(global_hash_table.slots[i].hash == 0) continue;
       printf("%u %u %u %u\n", global_hash_table.slots[i].hash, global_hash_table.slots[i].minv, global_hash_table.slots[i].maxv, global_hash_table.slots[i].sumv);
   }
}