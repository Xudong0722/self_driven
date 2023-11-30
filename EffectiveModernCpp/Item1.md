# Item-1.理解模板推导

之前一直忽略了这一块的知识，借这个机会查漏补缺。

第一章主要理解模板推导，auto推导，decltype的一些规则，帮助我们之后更好的使用模板，auto，以及参数的类型设计的更加合理！

我们先看看这个代码
```
template<class T>
void f(ParamType param){...}
```

当我们使用这个模板函数时，参数的类型不同，模板推导出的ParamType的类型也不同。那么对于常见的几种类型，模板参数是如何工作的？

```
int x = 100;       //int 
const int cx = x;  //const int
const int& rx = cx; //const int&   


template<class T>
void funa(T& param);

template<class T>
void funb(T&& param);

template<class T>
void func(T param);
```

以上三种模板函数，三个不同的变量传递进去，ParamType将会被推导成什么呢？

## 1.ParamType是一个引用，但不是通用引用
```
template<class T>
void funa(T& param);


int x = 100;       //int 
const int cx = x;  //const int
const int& rx = cx; //const int&   

funa(x);   // int&
funa(cx);  // const int&
funa(rx);  // const int&
```

通过结果可以看出来，对于变量`cx`和`rx`，他们的`const`属性都被推导出来了，`T`均被推导为`const int`

## 2.ParamType是一个通用引用

```
template<class T>
void funb(T&& param);


int x = 100;       //int 
const int cx = x;  //const int
const int& rx = cx; //const int&   

funb(x);   // int&    x是一个左值， T被推导成int&
funb(cx);  // int const&
funb(rx);  // int const&
funb(7);   // int&&
```


## 3.ParamType是非引用

```
template<class T>
void func(T param);

int x = 100;       //int 
const int cx = x;  //const int
const int& rx = cx; //const int&   

func(x);   // int
func(cx);  // int
func(rx);  // int
func(7);   // int
```

第三种应该是最简单的，重新拷贝了一个份新的，const，reference属性都没有。
即对于传值类型推导，`const`和`volatile`实参会被认为是`non-const`的和`non-volatile`的


## 4.数据退化和函数名退化