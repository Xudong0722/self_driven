#include<iostream>
#include <string>

class Data{
private:
    std::string str;
    static int cnt;
public:
    Data(std::string&& _str): str(std::move(_str)){
        std::cout << "Total cnt:" << ++cnt << '\n'; 
    }
};

int Data::cnt = 0;

int main()
{
    while(true){
        Data* data = new Data{std::string(100000,'a')};
        //delete data;
    }
    return 0;
}