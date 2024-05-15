#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Widget{
public:
    void SetPtr(std::unique_ptr<std::string>&& ptr){ 
        p = std::move(ptr);
    }
    void SetPtrV2(std::unique_ptr<std::string> ptr){
        p = std::move(ptr);
    }
private:
    std::unique_ptr<std::string> p;  //move-only
};

class Password{
public:
    explicit Password(std::string pwd):
        text(std::move(pwd)){}
    void changeTo(std::string new_pwd){
        text = std::move(new_pwd);  // 如果text.capacity() >= new_pwd.size(), 那么text的内存不需要释放，可以重用
    }
private:
    std::string text;
};

/*
优点：效率可以
缺点：需要维护两个函数，目标代码变大，代码膨胀
 */
class GadgetByRef{
public:
    void addName(std::string&& name){
        names.emplace_back(std::move(name));
    }
    void addName(const  std::string& name){
        names.emplace_back(name);
    }
private:
    std::vector<std::string> names;
};

/*
优点：没有重复代码，效率可以
缺点：模板展开可能会生成多个函数，并且还需要考虑T的类型
 */
class GadgetByURef{
public:
    template<typename T>
    void addName(T&& name){
        names.emplace_back(std::forward<T>(name));
    }
private:
    std::vector<std::string> names;
};

/*
优点：代码简单，易于维护
缺点：效率可能会稍差，在c++11中，如果addName的实参就是右值，那么也只是会移动两次，所以效率还可以，如果是左值，那么不可避免要拷贝一次
 */
class GadgetByVal{
public:
    void addName(std::string name){
        names.emplace_back(std::move(name));
    }
private:
    std::vector<std::string> names;
};
int main()
{
    /*
    Item41.对于移动成本低且总是被拷贝的可拷贝形参，考虑按值传递(Consider pass by value for copyable parameters that are cheap to move and always copied)
    
    对于移动成本低且总是被拷贝的可拷贝形参，考虑按值传递.
    Four reason:
    1.应该仅考虑使用传值方式。确实，只需要编写一个函数。只会在目标代码中生成一个函数。避免了通用引用的种种问题。但是毕竟开销会比那些替代方式更高
    2.仅考虑对于可拷贝形参使用按值传递。不符合此条件的的形参必须有只可移动的类型（move-only types）（的数据成员）
    3.按值传递应该仅考虑那些移动开销小的形参。当移动的开销较低，额外的一次移动才能被开发者接受
    4.你应该只对总是被拷贝的形参考虑按值传递。
     */


    //2
    Widget w;
    w.SetPtr(std::make_unique<std::string>("test"));   // 仅移动一次
    w.SetPtrV2(std::make_unique<std::string>("test")); //移动构造一次， 在实现中再移动一次，所以我们应仅考虑对于可拷贝形参使用按值传递

    //4
    std::string initPwd("Supercalifragilisticexpialidocious");
    Password p(initPwd);  //拷贝构造一次， 移动一次

    std::string newPassword = "Beware the Jabberwock";
    p.changeTo(newPassword);  //拷贝一次， 移动一次， 如果new password远高于初始密码，那么还会涉及内存的释放和重新分配


    // c++98中按值传递可能还有切片问题就，避免基类类型按值传参
    return 0;
}