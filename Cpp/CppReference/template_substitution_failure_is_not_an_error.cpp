/*
 * @Author: Xudong0722 
 * @Date: 2024-08-28 16:06:41 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-08-28 16:31:53
 */
#include <cstddef>
#include <iostream>

template<class T>
struct has_iterator_cat
{
private:
    struct two{
        char a;
        char b;
    };

    template<class U>
    static two trait(...);// {return two{};}
    template<class U>
    static char trait(typename U::special_obj* = nullptr);// { return char{};}

public:
    static constexpr bool value = (sizeof(trait<T>(nullptr)) == sizeof(char));
};

struct HasSpc{
    struct special_obj{};
};

struct NoSpc{

};

struct DerivedSpc : HasSpc{};

struct IntSpc{
    using special_obj = int;
};

int main()
{
    std::cout << has_iterator_cat<HasSpc>::value << std::endl;
    std::cout << has_iterator_cat<NoSpc>::value << std::endl;
    std::cout << has_iterator_cat<DerivedSpc>::value << std::endl;
    std::cout << has_iterator_cat<IntSpc>::value << std::endl;
    return 0;
}