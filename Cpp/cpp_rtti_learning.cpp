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
};

class Derived : public Base {
public:
    void virtualFunction() override {
        std::cout << "Derived::virtualFunction\n";
    }
};

int main() {
    Base baseObj;
    Derived derivedObj;

    // 获取 Base 类型对象的虚表地址
    uintptr_t* baseVtable = *reinterpret_cast<uintptr_t**>(&baseObj);
    std::cout << "Base vtable address: " << baseVtable << '\n';
    std::cout << ((std::type_info*)(baseVtable[-1]))->name() << '\n';

    // 获取 Derived 类型对象的虚表地址
    uintptr_t* derivedVtable = *reinterpret_cast<uintptr_t**>(&derivedObj);
    std::cout << "Derived vtable address: " << derivedVtable << '\n';
    std::cout << ((std::type_info*)(derivedVtable[-1]))->name() << '\n';

    return 0;
}