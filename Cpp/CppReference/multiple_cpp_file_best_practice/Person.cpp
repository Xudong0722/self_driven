// Person.cpp (实现文件)
#include "Person.h"
#include "PersonImpl.h"

Person::Person(const std::string& name, int age) : pImpl(std::make_unique<PersonImpl>(name, age)) {}

Person::~Person() = default;

std::string Person::getName() const {
    return pImpl->name;
}

int Person::getAge() const {
    return pImpl->age;
}
