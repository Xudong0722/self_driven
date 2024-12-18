# 内存布局

## 1.不含成员函数 - base.cpp

```c++
class BasePub{
public:
    int a;
    double b;
    int c;
    double d;
};

class BasePri{
    int a;
    std::string b;
    int c;
};
```

从gdb调试来说，public和private对成员变量的内存地址布局没有影响

我们看一下结果

```gdb
(gdb) p &a
$1 = (BasePub *) 0x61fdf0
(gdb) p &a.a
$2 = (int *) 0x61fdf0
(gdb) p &a.b
$3 = (double *) 0x61fdf8
(gdb) p &a.c
$4 = (int *) 0x61fe00
(gdb) p &a.d
$5 = (double *) 0x61fe08
(gdb) p &b 
$6 = (BasePri *) 0x61fdc0
(gdb) p &b.a
$7 = (int *) 0x61fdc0
(gdb) p &b.b
$8 = (std::__cxx11::string *) 0x61fdc8
(gdb) p &b.c 
$9 = (int *) 0x61fde8
(gdb) continue
```

测试环境 windows， g++
1.可以看出来对于BasePub来说，编译器实现了内存对齐，a的地址是0x61fdf0，b的地址是0x61fdf8，编译器填充了4个字节。
2.多次运行后发现每一次结果都是一样的，而windows平台是默认开启ASLR(地址空间布局随机化-Address Space Layout Randomization, 将进程中的某些内存空间地址进行随机化来增大入侵者预测目的地址的难度)
不知道为什么每次的地址都是一样的，TODO

## 2.含非virtual成员函数