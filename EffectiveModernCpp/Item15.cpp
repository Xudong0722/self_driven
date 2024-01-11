#include <iostream>
#include <array>
#include <type_traits>

template<typename E>
constexpr std::underlying_type_t<E> EnumToUtype(E enum_class) noexcept{
    return static_cast<std::underlying_type_t<E>>(enum_class);
}

int return_one(){
    return 1;
} 

constexpr int return_three(){  //编译器可知
    return 3;
}

enum class Fruit{
    //apple = return_one(),  //error: call to non-'constexpr' function 'int return_one()'
    orange = 2,
    watermelon = return_three(),  // no problem
};


class Point{
public:
    constexpr Point(double x_ = 0, double y_ = 0) noexcept : x(x_), y(y_){}
    
    constexpr double get_x() const noexcept {
        return x;
    }

    constexpr double get_y() const noexcept {
        return y;
    }

    //C++14支持 constexpr void xxx(...)
    constexpr void set_x(double x_) noexcept {
        x = x_;
    }

    constexpr void set_y(double y_) noexcept {
        y = y_;
    }

private:
    double x;
    double y;
};

constexpr Point get_mid_point(const Point& a, const Point& b) noexcept{
     return {(a.get_x() + b.get_x())/2, (a.get_y() + b.get_y())/2};
}

int main()
{
    //Item15.尽可能的使用constexpr

    {
        //constexpr修饰的变量是"编译器可知的"
        
        //array的大小
        int sz{0};
        //std::array<int, sz> arr1;       //error: the value of 'sz' is not usable in a constant expression
        constexpr int csz{101};
        std::array<int, csz> arr1; // no problem!

        //枚举名的值
        std::cout << EnumToUtype(Fruit::watermelon) << '\n';
    }


    {
        //constexpr修饰的函数可以用于计算编译期就需要的值
        //如果constexpr函数的实参不是编译期可知的，那么constexpr就是一个普通的函数
        //需要注意的是，如果你将一个函数声明成constexpr的话，调用方可能会利用你的返回值放在一些需要编译器常量的地方如array大小等
        //这是有风险的，如果你后面认为使用constexpr是一个错误，一旦修改就会造成大量的编译错误

        constexpr Point x;
        constexpr Point y(10, 10);
        constexpr auto mid = get_mid_point(x, y);   //编译器就可以得到mid
        std::cout << mid.get_x() << " " << mid.get_y() << '\n';  

        double xx{0}, yy{0};
        std::cin >> xx >> yy;
        Point tmp{xx, yy};
        auto mid2 = get_mid_point(x, tmp);   //运行期计算， get_mid_point 相当于一个普通的函数
        std::cout << mid2.get_x() << " " << mid2.get_y() << '\n';  
    }
    return 0;
}