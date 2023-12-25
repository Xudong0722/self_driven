#include <iostream>
#include <vector>

class Widget{

};

bool is_valid_data(const Widget& w){
    // do something 
    return true;
}

std::vector<bool> features(const Widget& w){
    std::vector<bool> res(5,true);
    //do something
    return res;
}
void processWidget(bool is_valid, const Widget& w){
    // do something
}

class Human{
    int age;
    
public:
    void eat(){};
    void drink(){};
};

int main(){

    //在一些情况下auto会出乎你的意料
    //such as, proxy class

    Human* elvis = new Human{};
    Human betsy;

    {
        Widget w;
        bool is_valid_w = features(w)[0]; // is_valid_w is bool 
        processWidget(is_valid_w, w);   // No problem.

        Widget u;
        auto is_valid_u = features(u)[0];  // is_valid_u is std::vector<bool> reference, it's a proxy class
        processWidget(is_valid_u, u);  // dangerous

        // std::vector<bool> reference 是一个代理类，并且是不期望用户可见的，所谓代理类，就是模仿并且增强一些类型的行为而存在的类
        // 在第一种情况下，实际上发生了一个reference -> bool 的转换，reference里面包含一个指针和一个偏移量
        // 为什么说第二种情况是危险的呢，因为is_valid_u里包含一个临时指针，features(u)[0]返回的临时对象销毁时，这个临时指针将会dangling
        // typedef unsigned long _Bit_type;
        // enum { _S_word_bit = int(__CHAR_BIT__ * sizeof(_Bit_type)) };

        // struct _Bit_reference
        // {
        //     _Bit_type * _M_p;
        //     _Bit_type _M_mask;

        //     operator bool() const _GLIBCXX_NOEXCEPT
        //     { return !!(*_M_p & _M_mask); }
        //     }
        //     //...
        // }
    }

    return 0;
}