/*
 * @Author: Xudong0722 
 * @Date: 2024-08-28 16:48:14 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-08-28 16:57:24
 */
#include <iostream>

int val{101};

int ChangeVal(){
    val = -101;
    return static_cast<int>(2.0);
}

#define var(x) #x << " = " << x

int main()
{   
    //判断下sizeof运算符会不会实际执行函数
    
    std::cout << var(val) << std::endl; 

    std::cout << sizeof(ChangeVal()) << std::endl;

    std::cout << var(val) << std::endl;
    return 0;
}

/*
Run result:

val = 101
4
val = 101

可以看出来sizeof运算符不会实际执行函数

*/