#include <iostream>
#include <string>
#include <vector>
#include <regex>

int main()
{
    /*
    Item42.考虑使用置入代替插入
    1.置入对比插入
    2.适用置入的情况
     */

    std::vector<std::string> vec;

    std::string str{"string"};
    
    
    vec.push_back(str);  // a
    vec.emplace_back(str);   // b
    //a, b两种方式并无差异， 都是调用一次拷贝构造函数 

    vec.push_back("xyz");  // c ： 先构造了一个临时对象，然后移动构造
    vec.emplace_back("xyz");  // d ： 原地构造， 这种情况下 置入优于插入
    vec.emplace_back(50, 'e');  //e : 原地构造， 完美转发参数给string的构造函数


    //哪些情况下，置入优于插入，取决于很多因素，根据经验满足以下条件，置入往往优于插入
    //a.值是通过构造函数添加到容器，而不是直接赋值

    vec.emplace(vec.begin(), "new");  //这种调用实际上是在头部插入一个string，而头部如果已经有元素的话，那么其实是赋值

    //b.传递的实参类型与容器的初始化类型不一样
    //根据调用23和24可以看出来， 实参并不是string， 而且可以构造string的实参

    //c.容器不拒绝重复项作为新值
    //这意味着容器要么允许添加重复值，要么添加的值都不是重复的，如果不可以重复的话，那么在置入时往往会新构造一个节点，然后拿这个节点
    //与现有的节点进行比较，如果没有，那么链接这个新节点

    //对于存储资源管理类，建议不要使用emplace

    //--------------------

    std::regex r1 = nullptr;   //拷贝构造初始化，不被允许调用explicit构造函数
    std::regex r2{nullptr};    //直接初始化，可以调用explicia构造函数
    
    //而emplace使用直接初始化，两种初始化的区别也导致了一些问题
    std::vector<std::regex> vec_reg;
    vec_reg.push_back(nullptr);  //error
    vec_reg.emplace_back(nullptr);  //no error!
    //这可能会导致一些难以发现的错误
    return 0;
}