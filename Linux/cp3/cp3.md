## 环境变量的坑

设置或者更新环境变量如果使用putenv的话要小心， 参数会被环境变量引用。如果这个参数是个局部变量，后续查询这个环境变量会受到参数生命周期的影响。
见test_env.c

使用setenv没有这个问题


## 动态库

1.生成动态库

```c++
#include <stdlib.h>
#include <stdio.h>

void dynamic_lib_call(void) {
    printf("dynamic lib call\n");
}
```

编译命令
```
gcc dlib.c -o libdlib.so -shared
```

2.链接动态库

```c++
#include <stdlib.h>
#include <stdio.h>

extern void dynamic_lib_call(void);

int main(void) {
    dynamic_lib_call();
    return 0;
}
```

编译命令
```
gcc test_dlib.c libdlib.so -o test
or
gcc test_dlib.c -ldlib -o test   //动态库一般命名libxxx.so， 这里-lxxx就是链接libxxx.so动态库
```

运行后会出错，因为实际运行的时候动态库的加载路径默认为/lib 和/usr/lib/
linux环境下可以通过/etc/ld.so.conf配置文件和环境变量LD_LIBRARY_PATH指示额外的动态库路径。

复制到/usr/lib/下之后就可以运行了
```
sudo cp libdlib.so /usr/lib/

./test
dynamic lib call
```


## 程序动态升级动态库

使用dlopen，dlsys来加载动态库，更新接口。重新申请新的内存，保存新的动态库接口。


## 避免内存问题

c库中一个不好的函数realloc， 它共有三种行为：

void *realloc(void* ptr, size_t size);
1. ptr为空，size不为空，等价于malloc(size)
2. ptr不为空，size为空。等价于free(ptr)
3. ptr不为空，size不为空。等价于free(ptr), malloc(size)

坑：
```c
void* ptr = realloc(ptr, new_size)
if(!ptr) {
    //...
}
```
如果ptr不为空，new_size不为0。就会分配内存，若失败返回NULL。但ptr原先指向的内存没有释放，造成内存泄露。