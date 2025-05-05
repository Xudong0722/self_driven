// Person.h (公共头文件，暴露给外部)
#ifndef PERSON_H
#define PERSON_H

#include <memory>
#include <string>

class PersonImpl;  // 前向声明

class Person {
public:
    Person(const std::string& name, int age);
    ~Person();

    std::string getName() const;
    int getAge() const;

private:
    std::unique_ptr<PersonImpl> pImpl;  // PImpl 指针
};

#endif  // PERSON_H
