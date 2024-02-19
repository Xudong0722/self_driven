#include <iostream>
#include <memory>
#include <functional>
#include <unordered_map>

class Widget{
private:
    int data;
public:
    Widget(int data_): data(data_){
        std::cout << "Widget::Widget(int data)\n";
    }
    ~Widget(){
        std::cout << "Widget::~Widget()\n";
    }
};

std::shared_ptr<const Widget> LoadWidget(int id){
    std::shared_ptr<const Widget> res = std::make_shared<const Widget>(id);
    return res;
}

std::shared_ptr<const Widget> LoadFastWidget(int id){
    static std::unordered_map<int, std::weak_ptr<const Widget>> cache;

    auto res = cache[id].lock();  //如果weak_ptr是空，res即为空， 否则返回shared_ptr

    if(nullptr == res){
        res = LoadWidget(id);
        cache[id] = res;
    }

    return res; 
}

class B;

class A{
public:
    std::shared_ptr<B> spa;
    A(){
        std::cout << __FUNCTION__ << '\n';
    }
    ~A(){
        std::cout << __FUNCTION__ << '\n';
    }
};

class B{
public:
    std::shared_ptr<A> spb;
    B(){
        std::cout << __FUNCTION__ << '\n';
    }
    ~B(){
        std::cout << __FUNCTION__ << '\n';
    }
};

class D;

class C{
public:
    std::shared_ptr<D> spc;
    C(){
        std::cout << __FUNCTION__ << '\n';
    }
    ~C(){
        std::cout << __FUNCTION__ << '\n';
    }
};

class D{
public:
    std::weak_ptr<C> spd;
    D(){
        std::cout << __FUNCTION__ << '\n';
    }
    ~D(){
        std::cout << __FUNCTION__ << '\n';
    }
};

int main()
{
    //Item20.当shared_ptr可能悬空时使用weak_ptr
    //场景： 缓存， 观察者模式， 打破shared_ptr的环状结构

    {
        //循环引用
        auto a = std::make_shared<A>();
        auto b = std::make_shared<B>();
        a->spa = b;
        b->spb = a;
        std::cout << a.use_count() << " " << b.use_count() << '\n';//2 2 互相引用， 无法释放，导致内存泄漏

        auto c = std::make_shared<C>();
        auto d = std::make_shared<D>();
        c->spc = d;
        d->spd = c;
        std::cout<< c.use_count() << " " << d.use_count() << '\n'; //1 2
        //weak_ptr 不会改变引用计数，它使用自己的计数， 所以spd对c的引用计数不影响，还是1
    }
    return 0;
}