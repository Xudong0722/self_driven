#include <memory>
#include <iostream>

using namespace std;

class A{
public:
    A(int a_, string b_, double* c_)
    : a(a_)
    , b(b_)
    , c(c_){}

    ~A(){
        cout <<"Over\n";
    }

    void print_a(){
        cout << a << endl;
    }

    void print_b(){
        cout << b << endl;
    }

    void print_c(){
        cout << c << " " << *c << endl;
    }

    void random_value(){
        a = 199;
        b = "jfdkslj";
        *c = 1010;
    }
private:
    int a{0};
    string b;
    double* c{nullptr};
};

int main()
{
    double tmp{3.1415926};
    shared_ptr<A> a = make_shared<A>(101, "test", &tmp);

    a->print_a();
    a->print_b();
    a->print_c();
    cout << a.get() << endl;
    cout << "--------------------------\n";

    auto b = a;
    b->print_a();
    b->print_b();
    b->print_c();
    cout << b.get() << endl;
    cout << "---------------------------\n";

    b->random_value();

    a->print_a();
    a->print_b();
    a->print_c();
    cout << a.get() << endl;
    cout << "----------------------------\n";
    return 0;
}