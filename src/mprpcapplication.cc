#include "mprpcapplication.h"

void MprpcApplication::Init(int argc, char **argv) { // 外部实现static方法不用加static

    //xxx    
}

MprpcApplication& MprpcApplication::GetInstance() {
    static MprpcApplication app;
    return app;
}

