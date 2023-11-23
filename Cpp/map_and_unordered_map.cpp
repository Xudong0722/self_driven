#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <string>

namespace elvis{
    class SpecialString{
    public:
        SpecialString(const std::string arg = ""): str(std::move(arg)) {
            std::cout<<"SpecialString():" <<arg<<'\n';
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


int main()
{
    std::map<elvis::SpecialString, int> string_map;
    elvis::SpecialString one{"1"};
    elvis::SpecialString two{"2"};
    elvis::SpecialString three_temp{"3"};
    elvis::SpecialString three{std::move(three_temp)};
    
    string_map.insert(std::pair<elvis::SpecialString, int>(three, 3));
    string_map.insert(std::pair<elvis::SpecialString, int>(one, 1));
    string_map.insert(std::pair<elvis::SpecialString, int>(two, 2));

    for(const auto& [key, value]: string_map){
        std::cout<<&key<<" "<<value<<'\n';
    }

    std::unordered_map<elvis::SpecialString, int, elvis::SpecialStringHash> string_unorder_map;
    string_unorder_map.insert(std::pair<elvis::SpecialString, int>(three, 3));
    string_unorder_map.insert({{one,1}, {two,2}});

    
    std::unordered_map<elvis::SpecialString, int, elvis::SpecialStringHash>::hasher hash_func = string_unorder_map.hash_function();
    for(const auto& [key, value]: string_unorder_map){
        std::cout<<&key<<" "<<value<<" "<<hash_func(key.GetName())<<'\n';
    }
    return 0;
}