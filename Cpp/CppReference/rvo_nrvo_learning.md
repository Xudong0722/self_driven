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

## 4.std::move

https://stackoverflow.com/questions/14856344/when-should-stdmove-be-used-on-a-function-return-value

不要过度使用std::move，比较下面两段代码

```c++
Test CreateTestDataWithNRVO(){
    Test test{100};
    //...
    return std::move(test);
}

[CreateTestDataWithNRVO] begin
[Test]100
[Test] 0x61fdcc
[Test Test&&] 0x61fe0c
[~Test] 0x61fdcc
[CreateTestDataWithNRVO] end
[~Test] 0x61fe0c
```

```c++
Test CreateTestDataWithNRVO(){
    Test test{100};
    //...
    return test;
}

[CreateTestDataWithNRVO] begin
[Test]100
[Test] 0x61fe0c
[CreateTestDataWithNRVO] end
[~Test] 0x61fe0c
```

哪一个代码的效率更高呢，在编译器优化之后，第二段代码的效率更高，对于返回值使用了std::move将会，破坏NRVO的条件(返回的local variable不统一)，阻止编译器的NRVO优化。
运行后，我们也能看出来，第一段代码进行了一次移动构造。

总结，RVO/NRVO都是为了减少临时对象的构造，直接再返回对象地址上构造。
NRVO的条件，所有返回的地方都需要是同名对象。
RVO是临时对象，不具名
注意， std::move会打破NRVO