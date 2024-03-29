#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"

int main(int argc, char** argv) {
    //初始化框架
    MprpcApplication::Init(argc, argv);

    //演示调用Rpc方法Login(同步)
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    fixbug::LoginResponse response;
    MprpcController controller;
    stub.Login(&controller, &request, &response, nullptr); // RpcChannel->CallMethod 

    //一次rpc调用完成，读结果
    if (controller.Failed()) {
        std::cout << controller.ErrorText() << std::endl;
    } else {
        if (response.result().errcode() == 0) {
            std::cout << "rpc login response success: " << response.success() << std::endl;
        } else {
            std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
        }
    }

    return 0;
}