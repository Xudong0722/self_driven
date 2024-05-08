#include <iostream>
#include <string>
#include <chrono>

class Node{
public:
    Node() = default;
    Node(const Node& node){
        a = node.a; 
        std::cout << "Node::const Node&" << '\n';
    }
    Node(Node&& node){
        a = std::move(node.a);
        std::cout << "Node::Node&&" << '\n';
    }
private:
    std::string a;
};

class Tree{
public:
    Tree(const Node node_): node(std::move(node_)){
        std::cout << "Tree::const Node" << '\n';
    }
private:
    Node node;
};

namespace elvis{

template<typename T>
constexpr std::remove_reference_t<T>&& move(T&& t) noexcept{
    return static_cast<std::remove_reference_t<T>&&>(t);
}
}


void process(const Node& node){
    std::cout << "process::Node&\n";
}

void process(Node&& node){
    std::cout << "process::Node&&\n";
}

template<typename T>
void logAndProcess(T&& param){
    auto now = std::chrono::system_clock::now().time_since_epoch();
    std::cout << __FUNCTION__ << now.count() << '\n';
    process(std::forward<T>(param));
}

int main()
{

    /*
    Item23.理解std::move和std::forward

    一. std::move
    1. move没有做任何移动，他是一个cast函数
    2. move只能保证你得到一个右值引用，它不会保证它转换的对象可以被移动
    */
    Node n;
    Tree{n};  // 虽然Tree的构造函数使用了std::move， 但是我们调用的还是拷贝构造函数，而不是移动构造函数，是因为Tree的构造函数使用了const
              // 我们看一下elvis::move, 如果T携带了const属性，那么最后的结果也会有const属性，而移动构造函数只接受non-const的右值引用

    
    /*
    二. std::forward

    
    */
    logAndProcess(Node{});  // 调用process(Node&&)
    logAndProcess(n);       // 调用process(const Node&)

    
    return 0;
}