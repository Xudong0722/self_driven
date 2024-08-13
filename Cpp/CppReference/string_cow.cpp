#include <iostream>
#include <string>

int main()
{
    /*
    Copy-on-write (COW),
    sometimes referred to as implicit sharing or shadowing, is a resource-management technique used in computer programming to 
    efficiently implement a "duplicate" or "copy" operation on modifiable resources (most commonly memory pages, storage sectors, files, and data structures).
    写时复制- c++11开始已经废弃
    一个字符串拷贝的优化策略，在进行字符串拷贝时，两个字符串先共享，等待某个字符串真正使用字符串时，再进行复制
    */

    std::string source{"hello"};
    std::string new_str = source;  //此时，这两个字符串共享"hello"

    new_str[0] = 'y';  //此时，因为涉及到修改，我们不能影响到source，所以拷贝原数据到一块新的内存中再进行修改


    //c++11后开始废弃， 21.4.1 [string.require] says that the non-const forms of operator[], at, front, back, begin, rbegin, end and rend may not invalidate references, pointers and iterators (so must not reallocate or modify the string)
    //而COW实现的string可能会导致其失效

    std::string s{"str"};
    const char* p = s.data();
    {
        std::string s2(s);
        (void)s[0];  //operator[]会导致s需要一份新的数据
    } //s2析构掉, 所以会导致p成为悬空指针
    std::cout << p << '\n';  //undefined behavior before c++11
    return 0;
}