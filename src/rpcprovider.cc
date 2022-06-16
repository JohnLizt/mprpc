#include "rpcprovider.h"
#include "mprpcapplication.h"



void RpcProvider::NotifyService (::google::protobuf::Service *service) {
    ServiceInfo service_info;
    
    //获取服务信息（protobuf提供的Service基类方法）
    const google::protobuf::ServiceDescriptor* pserviceDesc = service->GetDescriptor();
    //获取服务名
    std::string service_name = pserviceDesc->name();
    //获取服务对象service的方法数量
    int methodCnt = pserviceDesc->method_count();

    for (int i = 0; i < methodCnt; i++)
    {
        //获取第i个服务方法的描述
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name,pmethodDesc});
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name,service_info});
}

    
void RpcProvider::Run() {
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);

    // 创建TcpServer
    std::string serverName = "RpcProvider";
    muduo::net::TcpServer server(&m_eventLoop, address, serverName);
    // 绑定连接回调和消息读写回调方法，分离了网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 设置muduo库的线程数量:1个IO线程，3个工作线程
    server.setThreadNum(4);

    // rpc服务端准备启动，打印信息
    std::cout << "RpcProvider start service at ip:" << ip << " port:" << port << std::endl;

    // 启动网络服务
    server.start();
    m_eventLoop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {

}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& , muduo::net::Buffer*, muduo::Timestamp) {

}