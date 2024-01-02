#include <iostream>
#include <tuple>
#include <type_traits>

//C++11 version
template<typename E>
constexpr typename std::underlying_type<E>::type user_info_type_c11(E enumerator) noexcept{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

//C++14 version
template<typename E>
constexpr auto user_info_type_c14(E enumerator) noexcept{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

int main()
{
    //Item9: 尽量使用scoped enum
    {
        //1.避免命名空间污染
        enum Color{white, red, yellow,};
        
        //auto red = false; //error, redeclared
        
        {
            enum class Color{white, red, yellow};

            auto white = "white";  // No problem.

            auto color = Color::yellow;
        }
    }

    {
        //2.避免隐式转换
        //对于scoped enum, class name是强类型，但是对于unscoped enum,它可以隐式转换为其他类型

        enum Color{white, red, yellow};
        auto func = [](double x){
            std::cout << "x = " << x << '\n';
        };

        func(red); // no compile error, but no sense

        enum class Food{rice, noodles, bread = 0xFFFF};
        //func(Food::bread); //error: no match for call to '(main()::<lambda(double)>) (main()::Food)'
        //if you want to call func with Food, use static_cast
        func(static_cast<double>(Food::bread));
    }

    {
        //3.unscoped enum不可以前置声明，因为编译器会根据enum内部实际定义的变量来决议类型
        enum Test1{test1_1};
        enum Test2{test2_1, test2_2 = 0xFFFFFFFFFF};

        std::cout << sizeof(Test1) << " " << sizeof(Test2) << std::endl; // 输出 4 8. Test1和Test2在底层不是一个类型，所以我们不可以前置声明unscoped enum，除非指定底层类型
        //enum Test3; //error: use of enum 'Test3' without previous declaration
        enum Test4:uint32_t;  //fine, 指定类型后可以使用前置声明
        auto func = [](Test4 x){
            std::cout << "Test4 x = " << x << '\n';
        };

        enum Test4:uint32_t{
            test4_1 = 101,
            test4_2 = 0xeeeeee,
        };
        
        func(test4_2);

        enum class Test5;  //ok，支持前置声明，底层类型有默认值
        auto fund = [](Test5 x){
            std::cout << "Test5 x = " << static_cast<uint16_t>(x) << '\n';
        };

        enum class Test5{
            test5_1 = 103,
            test5_2 = 1003,
            //test5_3 = 0xFFFFFFFFFF, //error: enumerator value '17592186044415' is outside the range of underlying type 'int'
            // 由此可以，enum class是由默认的底层类型的，如需扩展，我们可以指定underlying type
        };
        fund(Test5::test5_1);
    }

    {
        //Corner Case
        using user_info = std::tuple<std::string, std::string, std::size_t, std::size_t>; // name, e-mail, age, asset
        user_info betsy = std::make_tuple("betsy", "betsy@123.com", 18, 1000000);
        
        const auto betsy_name = std::get<0>(betsy);  // wield！ magic number

        enum UserInfo{name, email, age, asset};
        const auto betsy_email = std::get<email>(betsy);  // Make sense! 利用枚举可以隐式转换的特性

        enum class UserInfoV2{name, email, age, asset};
        //如果担心命名空间污染，可以使用scoped enum, 不过代码看起来很冗余
        const auto betsy_age = std::get<static_cast<std::size_t>(UserInfoV2::age)>(betsy);

        // 我们可以定义一个函数，传入枚举名，返回对应的底层类型的值
        const auto betsy_asset = std::get<user_info_type_c14(UserInfoV2::asset)>(betsy);
        std::cout << betsy_name << " " << betsy_email << " " << betsy_age << " " << betsy_asset << '\n';
    }
    return 0;
}