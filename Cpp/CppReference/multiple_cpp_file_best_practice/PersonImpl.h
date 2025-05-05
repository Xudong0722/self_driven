// PersonImpl.h (内部头文件，仅供实现文件使用)
#ifndef PERSONIMPL_H
#define PERSONIMPL_H

#include <string>

class PersonImpl {
public:
    PersonImpl(const std::string& name_, int age_) : name(name_), age(age_) {}
    
    std::string name;
    int age;
    double kg{0.0};
};

#endif  // PERSONIMPL_H
