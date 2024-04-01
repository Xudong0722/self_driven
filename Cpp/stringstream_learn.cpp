#include <iostream>
#include <sstream>
#include <memory>

class ss{
public:
    std::stringstream& getSS() { return m_ss; }
    const std::string getContent() const { return m_ss.str(); }
private:
    std::stringstream m_ss;
};

int main()
{
    auto t = std::make_shared<ss>();
    t->getSS() << "hello";

    std::cout << t->getContent() << '\n';
    return 0;
}