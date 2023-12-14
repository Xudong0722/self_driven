// #include <iostream>
// using namespace std;

// class X
// {
//     public:
//         X()
//         {
//             mX = 101;
//         }
//         virtual void vfunc()
//         {
//             cout << "X::vfunc()" << endl;
//         }
//     private:
//         int mX;
// };

// class XX : public X
// {
//     public:
//         XX():
//             X()
//         {
//             mXX = 1001;
//         }
//         virtual void vfunc()
//         {
//             cout << "XX::vfunc()" << endl;
//         }
//     private:
//         int mXX;
// };

// typedef void (*FuncPtr)();
// int main()
// {
//     X xx;
//     FuncPtr func;
//     char *p = (char*)&xx;
//     // 获得虚函数表的地址
//     int **vtbl = (int**)*(int**)p;
//     // 输出虚函数表的地址，即vptr的值
//     cout << vtbl << endl;
//     // 获得type_info对象的指针，并调用其name成员函数
//     cout << "\t[-1]: " << (vtbl[-1]) << " -> "
//         << ((type_info*)(vtbl[-1]))->name() << endl;
//     // 调用第一个virtual函数
//     cout << "\t[0]: " << vtbl[0] << " -> ";
//     func = (FuncPtr)vtbl[0];
//     func();
//     // 输出基类的成员变量的值
//     p += sizeof(int**);
//     cout << *(int*)p << endl;
//     // 输出派生类的成员变量的值
//     p += sizeof(int);
//     cout << *(int*)p << endl;
//     return 0;
// }

#include <iostream>

class Base {
public:
    virtual void virtualFunction() {
        std::cout << "Base::virtualFunction\n";
    }

    virtual void printHello() {
        std::cout << "Base::Hello\n";
    }
};

class Derived : public Base {
public:
    void virtualFunction() override {
        std::cout << "Derived::virtualFunction\n";
    }
};

int main() {
    Base base_obj;
    Derived derived_obj;

    // 获取 Base 类型对象的虚表地址
    // 这里使用unsigned long long 是为了可以拿到任何类型的指针
    uintptr_t* base_vtable = *reinterpret_cast<uintptr_t**>(&base_obj);
    std::cout << "Base vtable address: " << base_vtable << '\n';

    // 获取Base 类型对象的type_info指针
    // std::type_info* type_info_base= reinterpret_cast<std::type_info*>(base_vtable[-1]);
    std::type_info* type_info_base= reinterpret_cast<std::type_info*>(*(base_vtable-1));
    std::cout << type_info_base->name() << '\n';

    // 直接通过vptr调用虚函数
    using pfunc = void(*)();
    pfunc base_virtual_func = reinterpret_cast<pfunc>((void*)(base_vtable[0]));
    pfunc base_print_hello = (pfunc)base_vtable[1];
    base_virtual_func();
    base_print_hello();
    
    // 获取 Derived 类型对象的虚表地址
    uintptr_t* derivedVtable = *reinterpret_cast<uintptr_t**>(&derived_obj);
    std::cout << "Derived vtable address: " << derivedVtable << '\n';
    // 获取Derived 类型对象的type_info指针
    std::cout << ((std::type_info*)(derivedVtable[-1]))->name() << '\n';


    int a[] = {-4, -3, -2, -1};
    int b[] = {0, 1, 2, 3};
    int c[] = {4, 5, 6, 7};
    
    int* pb = b;
    std::cout << a << " " << b << " " << c << '\n';
    std::cout << *(pb+1) << " " << *(pb-1) << " " << pb[-1] << '\n';
    return 0;
}
