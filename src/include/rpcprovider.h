#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <string>
#include <functional>
#include <google/protobuf/descriptor.h>
#include <unordered_map>

// 框架提供的专门用来发布rpc服务的类
class RpcProvider
{
public:
    // 这里是框架提供给外部使用的
    void NotifyService (::google::protobuf::Service *service); // 这里不能用UserService子类，而要用Service基类指针（父类指针指向任何一个子类实现多态）

    // 启动rpc服务节点，开始提供rpc远程调用服务
    void Run();

private:
    // 组合EventLoop
    muduo::net::EventLoop m_eventLoop;

    //Service信息
    struct ServiceInfo {
        google::protobuf::Service* m_service; //保存服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*> m_methodMap; // 方法映射表
    };
    std::unordered_map<std::string,ServiceInfo> m_serviceMap;

    //新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr&);
    //已建立连接用户的读写事件回调
    void OnMessage(const muduo::net::TcpConnectionPtr& , muduo::net::Buffer*, muduo::Timestamp);
    //Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);
};
