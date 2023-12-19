# RVO & NRVO

## 1.C++11 with compiler optimization

```c++
Output:
[Test] 0x61ff1c
[CreateTestDataWithNRVO] end
[CreateTestDataWithRVO] begin
[Test] 0x61ff18
[CreateTestDataWithRVO] end
[~Test] 0x61ff18
[~Test] 0x61ff1c
```

可以发现，在C++11中，如果开启编译器优化，不管是RVO和NRVO都是直接在main函数的栈上直接构造，没有多余的拷贝构造和移动构造

## 2.C++11 without compiler optimization

我们可以通过编译器指令`-fno-elide-constructors`进行禁止`RVO/NRVO`

```c++
Output:
[CreateTestDataWithNRVO] begin
[Test] 0x61fedc
[Test Test&&] 0x61ff08
[~Test] 0x61fedc
[Test Test&&] 0x61ff04
[~Test] 0x61ff08
[CreateTestDataWithNRVO] end
[CreateTestDataWithRVO] begin
[Test] 0x61fedc
[Test Test&&] 0x61ff0c
[~Test] 0x61fedc
[Test Test&&] 0x61ff00
[~Test] 0x61ff0c
[CreateTestDataWithRVO] end
[~Test] 0x61ff00
[~Test] 0x61ff04
```

## 3.C++17 with compiler optimization(RVO) in default

https://zhuanlan.zhihu.com/p/588852649
在上面这篇博客中提到，C++17默认开启RVO，就算显示指定编译器指令也无法生效，但是在本机验证上时，发现指定编译器指令后，输出仍和tips2一样，可能和编译器版本有关系，这里有待考证。

## 4.原理

在《深入探索C++对象模型》第2章构造函数语意学第3节程序转化语意学中，作者提到，对于返回值优化，编译器层面做的优化是，修改函数原型，新增一个对象的引用参数，类似下面这种方式

```c++
//NRVO: Named return value optimization
Test CreateTestDataWithNRVO(){
    Test test{100};
    return test;
}

//伪代码
//NRVO: Named return value optimization
void CreateTestDataWithNRVO(Test& __result){
    __result.Test::Test(100);
}

main()
{
    Test test; 
    CreateTestDataWithNRVO(test);
}
```

简单来说，就是直接在main函数局部对象test的地址上构造，而不需要借助临时对象，然后再执行拷贝构造或者移动构造。

在这篇博客中，https://blog.csdn.net/Howl_1/article/details/122625409， 作者查看了函数的堆栈调用，得出结论，其实在函数中构造了一个对象，然后用main函数中局部对象test的地址接住了这个临时对象，并通过标记位跳过临时对象的析构。