/*
    Result:
    emplace_back:
    Create an Engineer

    push_back:
    Create an Engineer
    Move an Engineer

    Mike is working in Web, he/she was recruited at 1987.
    Duke is working in Android, he/she was recruited at 1936.


    // emplace_back 支持在append的时候在相应的位置原地构造
    // 而push_back不支持原地构造，只能构造完移动
*/

#include <vector>
#include <cassert>
#include <iostream>
#include <string>
 
struct Engineer{
    std::string name;
    std::string department;
    int recruit_year;

    Engineer(std::string _name, std::string _department, int year):
        name(std::move(_name)), department(std::move(_department)), recruit_year(year)
    {
        
        std::cout<<"Create an Engineer\n";
    }

    Engineer(Engineer&& engineer):
        name(std::move(engineer.name)), department(std::move(engineer.department)), recruit_year(engineer.recruit_year) 
    {
        std::cout<<"Move an Engineer\n";
    }
};
 
int main()
{
    std::vector<Engineer> zone;
    std::cout << "emplace_back:\n";
    auto& ref = zone.emplace_back("Mike", "Web", 1987);
    assert(ref.recruit_year == 1987 && "uses a reference to the created object (C++17)");
 
    std::vector<Engineer> re_zone;
    std::cout << "\npush_back:\n";
    re_zone.push_back(Engineer("Duke", "Android", 1936));
    
    {
        std::cout << "\n";
        for (auto const& engineer: zone)
            std::cout << engineer.name << " is working in "
                    << engineer.department << ", he/she was recruited at " << engineer.recruit_year << ".\n";
    
        for (auto const& engineer : re_zone)
            std::cout << engineer.name << " is working in "
                    << engineer.department << ", he/she was recruited at " << engineer.recruit_year << ".\n";
    }
    return 0;
}