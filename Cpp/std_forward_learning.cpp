#include <iostream>
#include <utility>
#include <string>

namespace elvis{
    class SpecialString{
    public:
        SpecialString(const char* arg): str(arg){
            std::cout<<"SpecialString(const char*):" <<arg<<'\n';
        }

        SpecialString(const std::string& arg): str(arg) {
            std::cout<<"SpecialString(const std::string&):" <<arg<<'\n';
        }
        SpecialString(std::string&& arg): str(arg) {
            std::cout<<"SpecialString(const std::string&&):" <<arg<<'\n';
        }

        SpecialString(const SpecialString& elem) {
            this->str = elem.str;
            std::cout<<"SpecialString(const SpecialString&)\n";
        }

        SpecialString(SpecialString&& elem){
            this->str = std::move(elem.str);
            std::cout<<"SpecialString(SpecialString&&)\n";
        }

        ~SpecialString(){
            std::cout<<"~SpecialString()\n";
        }

        bool operator<(const SpecialString& elem) const{
            std::cout<<"SpecialString::operator<(const SpecialString&)\n";
            return this->str < elem.str;
        }

        SpecialString& operator=(SpecialString&& elem){
            std::cout<<"SpecialString::operator==(SpecialString&&)\n";
            this->str = std::move(elem.str);
            return *this;
        }

        size_t operator()(const SpecialString& elem) const {
            std::cout<<"SpecialString::operator()(const SpecialString&)\n";
            return std::hash<std::string>{}(elem.str);
        }

        bool operator==(const SpecialString& elem) const{
            std::cout<<"SpecialString::operator==(cosnt SpecialString&)\n";
            return this->str == elem.str;
        }

    public:
        std::string GetName() const {
            return this->str;
        }
    private:
        std::string str;
    };

    class SpecialStringHash{
    public:
        size_t operator()(const SpecialString& elem) const{
            std::cout<<"SpecialStringHash::operator(const SpecialString&)\n";
            return std::hash<std::string>{}(elem.GetName());
        }

    };
}

template<typename T>
elvis::SpecialString* SpecialStringFactory(T&& str){
    return new elvis::SpecialString{str};
}

template<class T>
elvis::SpecialString* SpecialStringFactoryWithPerfectForwarding(T&& str){
    return new elvis::SpecialString{std::forward<T>(str)};
}


int main()
{
    std::string left_value{"This is a left value"};
    std::string right_dec{"? no, it's a right value"};
    
    {
        std::cout<< "no forward begin\n";
        elvis::SpecialString* no_forward_by_left_value = SpecialStringFactory(left_value);
        elvis::SpecialString* no_forwark_by_right_value = SpecialStringFactory(left_value + right_dec);
        delete no_forward_by_left_value;
        delete no_forwark_by_right_value;
        no_forward_by_left_value = nullptr;
        no_forwark_by_right_value = nullptr;
        std::cout<< "no forward end\n";
    }

    {
        std::cout<< "perfect forward begin\n";
        elvis::SpecialString* perfect_forward_by_left_value = SpecialStringFactoryWithPerfectForwarding(left_value);
        elvis::SpecialString* perfect_forwark_by_right_value = SpecialStringFactoryWithPerfectForwarding(left_value + right_dec);
        delete perfect_forward_by_left_value;
        delete perfect_forwark_by_right_value;
        perfect_forward_by_left_value = nullptr;
        perfect_forwark_by_right_value = nullptr;
        std::cout<< "perfect forward end\n";
    }

    return 0;
}

/*
result:
no forward begin
SpecialString(const std::string&):This is a left value
SpecialString(const std::string&):This is a left value? no, it's a right value
~SpecialString()
~SpecialString()
no forward end
perfect forward begin
SpecialString(const std::string&):This is a left value
SpecialString(const std::string&&):This is a left value? no, it's a right value
~SpecialString()
~SpecialString()
perfect forward end


 we can find that it called SpecialString(std::string&& arg) when we use std::forward, which remains the right-value property!
*/
