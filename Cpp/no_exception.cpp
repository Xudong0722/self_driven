#include <iostream>
#include <memory>

class A{
public:
    A(){
        std::cout << "A::A()\n";
    }
    ~A(){
        std::cout << "A::~A()\n";
    }
};

void throw_1(){
    A a;
    throw 1;
}

void block_throw(){
    A a;
    throw "block throw";
}

//noexpect修饰的函数如果有异常走出来(函数内没有被捕捉)就会直接调用terminate来终止程序，这样就有内存泄漏的可能
void no_block_throw() noexcept{
    A a;
    try{
        throw "no block throw";
    }catch(...){
        std::cout << "catch this ex\n";
    }
    
}

class PA{
public:
    PA() : re(std::make_unique<A>()){
        std::cout << "PA::PA()\n";
    }
    ~PA() {
        std::cout << "PA::~PA()\n";
    }

private:
    std::unique_ptr<A> re;
};

int main()
{
    int op{0};

    std::cin >> op;
    try{
        switch(op){
        case 1: {
            throw_1();  //没有catch， 就会terminate
            break;
        }
        case 2:{
            PA a;
            block_throw(); //没有catch， 就会terminate
            break;
        }
        case 3:{
            no_block_throw();
            break;
        }
        case 4:{
            PA a;
            no_block_throw();
        }
        default:break;
        }
    }catch(...){   //如果没有catch，那么所有没被捕捉的异常都会导致terminate
        std::cout << "catch it" << op << std::endl;
    }

    return 0;
}