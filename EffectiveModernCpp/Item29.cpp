#include <iostream>
#include <string>
#include <vector>


int main()
{
    /*Item29. 假定移动操作不存在，成本高，未被使用*/
    int high_stack_address{1000};
    std::cout << &high_stack_address << '\n';
    int* low_heap_address = new int(100);
    std::cout << low_heap_address << '\n';

    std::vector<int> a{1,2,3};
    std::vector<int> ma = std::move(a);     // 像这种情况， 的确移动操作会比较快， 因为只需要将a中的指针拷贝给ma，在将a中的指针置为空即可，常数时间，但是如果我们容器内存放的是一个自定义的类，那么移动和拷贝的性能对比就取决于这个类的移动构造的实现了

    for(int i : a){
        std::cout << i << ' ';  //Nothing
    }

    for(int i : ma){
        std::cout << i << ' ';  // 1 2 3
    }


    std::string s{"cow"};
    std::string cs = s;

    std::cout << &s << ' ' << &cs << '\n';

    std::string small_string {"samll"};
    std::string big_string(1010, '1');

    printf("%p, %p\n", &small_string, small_string.c_str());  // 小字符串，这里是存放在栈上的， small string optimization优化
    printf("%p, %p\n", &big_string, big_string.c_str());   // 存放在堆上


    int low_stack_address{1};
    std::cout << &low_stack_address << '\n';
    int* high_heap_address = new int(101);
    std::cout << high_heap_address << '\n';

    delete low_heap_address;
    delete high_heap_address;

    //存在几种情况，移动操作并无优势
    //1.要移动的对象不支持移动操作，移动写法等价于拷贝操作
    //2.要移动的对象移动不会比拷贝更快，
    //3.移动操作不可用，比如上下文要求移动操作不会抛出异常，但是移动操作没有声明成noexcept
    return 0;
}