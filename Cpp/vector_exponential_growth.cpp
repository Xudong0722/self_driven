/*
append_count = 10000:
1 vec_cap : 1
2 new_vec_cap : 2
3 new_vec_cap : 4
4 new_vec_cap : 8
5 new_vec_cap : 16
6 new_vec_cap : 32
7 new_vec_cap : 64
8 new_vec_cap : 128
9 new_vec_cap : 256
10 new_vec_cap : 512
11 new_vec_cap : 1024
12 new_vec_cap : 2048
13 new_vec_cap : 4096
14 new_vec_cap : 8192
15 new_vec_cap : 16384


append_count = 100000:
1 vec_cap : 1
2 new_vec_cap : 2
3 new_vec_cap : 4
4 new_vec_cap : 8
5 new_vec_cap : 16
6 new_vec_cap : 32
7 new_vec_cap : 64
8 new_vec_cap : 128
9 new_vec_cap : 256
10 new_vec_cap : 512
11 new_vec_cap : 1024
12 new_vec_cap : 2048
13 new_vec_cap : 4096
14 new_vec_cap : 8192
15 new_vec_cap : 16384
16 new_vec_cap : 32768
17 new_vec_cap : 65536
18 new_vec_cap : 131072


The current exponential is 2
*/



#include <iostream>
#include <vector>

static int cnt = 0;
#define LOG(x) std::cout << ++cnt <<" "<< #x <<" : "<< x << std::endl;

int main()
{
    std::vector<int> vec(1,0);

    int vec_cap = vec.capacity(), vec_size = vec.size();
    LOG(vec_cap);
    //LOG(vec_size);

    constexpr int append_count = 100000;
    for(int i = 0; i < append_count; ++i){
        vec.push_back(i);
        int new_vec_cap = vec.capacity();
        int new_vec_size = vec.size();

        if(new_vec_cap != vec_cap){
            LOG(new_vec_cap);
        }
        // if(new_vec_size != vec_size){
        //     LOG(new_vec_size);
        // }
        vec_cap = new_vec_cap;
        vec_size = new_vec_size;
    }

    return 0;
}