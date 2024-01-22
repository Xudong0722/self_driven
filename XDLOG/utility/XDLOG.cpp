#include "XDLOG.h"
#include <ctime>
#include <iostream>
#include <string>
#include <cassert>
#include <cstdarg>

using namespace utility::elvis;

XDLOG::XDLOG(){

}

XDLOG::~XDLOG(){

}

XDLOG& XDLOG::GetInst(){
    static XDLOG xdlog;
    return xdlog;
}

void XDLOG::log(LOG_LEVEL log_level, const char* function_name, ...)
{
    time_t raw_time;
    time(&raw_time);
    struct tm* ptm = localtime(&raw_time);
    
    std::string time_info_temp = asctime(ptm);
    if(time_info_temp.back() == '\n'){
        time_info_temp.pop_back();
    }
    std::string log_time_info = "[" + time_info_temp + "]";
    
    assert(log_level>NONE && log_level <= TRACE);
    std::cout << log_time_info << " " << LOG_LEVEL2STR[log_level] << " " << function_name << "\n";

    va_list arg_list;
}


template<typename... Args>
void XDLOG::log_test(Args ... args)
{
    std::cout << "Number of arguments: " << sizeof...(args) << std::endl;
    std::cout << "Arguments: ";
    //((std::cout << args << " "), ...);
    std::cout << std::endl;
}


