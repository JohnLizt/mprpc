#pragma once
#include "google/protobuf/service.h"

// 框架提供的专门用来发布rpc服务的类
class RpcProvider
{
public:
    // 这里是框架提供给外部使用的
    void NotifyService (::PROTOBUF_NAMESPACE_ID::protobuf::Service *service); // 这里不能用UserService子类，而要用Service基类指针（父类指针指向任何一个子类实现多态）

    // 启动rpc服务节点，开始提供rpc远程调用服务
    void Run();
};
