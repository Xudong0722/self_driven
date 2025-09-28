#include <map>
#include <iostream>

using namespace std;

class A{
public:
    A() {cout << "A:()\n";}
    ~A() {cout << "~A(a=" << a << ")\n";}
    A(int _a): a(_a) { cout << "A:(_a)\n"; }

    bool operator<(const A& rhs) const{
        return a < rhs.a;
    }
private:
    int a{0};
};

int main()
{
    map<A, int> h;
    
    cout << "-------------\n";
    A a(101);
    cout << "-------------\n";
    
    //h.emplace(make_pair(a, 1001));
    //h.emplace(make_pair(a, 1002));

    cout << "-------------\n";
    h.try_emplace(a, 1003);
    h.try_emplace(a, 10004);

    return 0;
}