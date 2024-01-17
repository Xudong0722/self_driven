#include "XDLOG.h"

using namespace utility;

elvis::XDLOG::XDLOG(){

}

elvis::XDLOG::~XDLOG(){

}

elvis::XDLOG& elvis::XDLOG::GetInst(){
    static XDLOG xdlog;
    return xdlog;
}

