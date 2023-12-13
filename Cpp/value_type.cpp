#include <iostream>

#define PRINT(x) std::cout << "The address of " << #x << " is " << &x << '\n';

template<typename _Tp>
constexpr typename std::remove_reference<_Tp>::type&&
Move(_Tp&& __t) noexcept
{ return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }



/*
泛左值 (glvalue)（“泛化 (generalized)”的左值）是一个表达式，其值可确定某个对象或函数的标识；
纯右值 (prvalue)（“纯 (pure)”的右值）是求值符合下列之一的表达式：
    计算某个运算符的操作数的值（这种纯右值没有结果对象）
    初始化某个对象（称这种纯右值有一个结果对象）。
    结果对象可以是变量，由 new 表达式创建的对象，由临时量实质化创建的临时对象，或者前述三类对象的成员。注意非 void 的弃值表达式有结果对象（即被实质化的临时量）。并且在作为 decltype 的操作数以外的情况下，每个类类型或数组类型的纯右值都有结果对象；
亡值 (xvalue)（“将亡 (expiring)”的值）是代表它的资源能够被重新使用的对象或位域的泛左值；
左值 (lvalue)（如此称呼的历史原因是，左值可以在赋值表达式的左边出现）是非亡值的泛左值；
右值 (rvalue)（如此称呼的历史原因是，右值可以在赋值表达式的右边出现）是纯右值或者亡值。
*/
int main()
{
    {
        //lvalue
        int x{0};
        PRINT(x);
        std::string str{"hello"};
        std::cout << "Before move, str is " << str << '\n';
        std::string str_construct_by_rref{std::move(str)};   
        std::cout << "After move, str is " << str << " The str_construct_by_rref is " << str_construct_by_rref << '\n'; // 移动语义体现！ After move, str is  The str_construct_by_rref is hello
        std::string&& rref_str = std::move(str_construct_by_rref); // 右值引用是左值， notice: 右值引用出现的原因是为了移动语义和实现C++的完美转发！！！一般地，这里将所有权转移给rref_str之后，不应该再使用str_construct_by_rref了
        std::cout << "rref_x address : " << &rref_str << '\n';
        std::cout << "(++x) address : " << &++x << '\n';    // 左目自增为lvalue
        std::cout << "(x=1) address : " << &(x=1) << '\n';
        std::cout << "(string view) address : " << &("string view") << '\n'; //字符串字面值是lvalue
        std::cout << "(std::endl) address is : " << &std::endl << '\n';
    }

    {
        //prvalue
        int x{0};
        // x + 1
        // 除字符串字面值之外的字面值： true(bool), false(bool), 101(int)...
        // x ++ ， 右目自增为prvalue， 用临时变量先存储了x，然后x自增，返回临时变量
    }

    {
        // xvalue(expiring value)
        // 返回类型是对象的右值引用的函数调用或重载运算符表达式， such as std::move()
        // 转换到对象的右值引用类型的类型转换表达式，such as static_cast<char&&>(x)
        // 它可以是广义上的左值： 将亡值是放在可写的数据区上的，所以如果你知道将亡值的地址，可以修改它，那它就可以认为是左值
        // 一般情况下，它就是右值，如果你遵守规则不去修改它，它就是完成某些操作由编译器生成的中间产物，对我们来说并不可用，就是右值       
    }
    return 0;
}