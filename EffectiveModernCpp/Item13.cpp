#include <iostream>
#include <vector>
#include <algorithm>

namespace elvis{

    template<typename C>
    auto cbegin(const C& container) -> decltype(std::begin(container)){
        return std::begin(container);
    }

    template<typename C>
    auto cend(const C& container) -> decltype(std::end(container)){
        return std::end(container);
    }
}


int main()
{
    //Item13.优先考虑const_iterator而非iterator
    //1.优先考虑const_iterator而非iterator
    //2.在最大程度通用的代码中，优先考虑非成员函数版本的begin，end，rbegin等，而非同名成员函数


    {
        // 如果我们不需要更改迭代器所指向的内容，就使用const_iterator - 合理使用const，一般问题都发生在变化中
        std::vector<int> arr{1,2,4,54,654};
        // auto it = std::find(cbegin(arr), cend(arr), 54);  // C++11 doesn't support cbegin(...)
        // *it = 666;  //"it" is const_iterator


        auto it = std::find(arr.cbegin(), arr.cend(), 54);
        if(it != arr.cend()) std::cout << *it << std::endl;

        //我们可以定义一个cbegin函数
        auto itx = std::find(elvis::cbegin(arr), elvis::cend(arr), 654);
        if(itx != cend(arr)) std::cout << *itx << std::endl;
        //*itx = 666;  //"itx" is const_iterator
    }

    {
        //对于第二点，对于一个类，成员函数越多，意味着他对外的接口更多，意味着更多的不确定性
        //为了降低这种不确定性，同时非成员函数明显可扩展性强，我们尽量使用非成员函数版本的begin,end,cbegin...
    }
    return 0;
}