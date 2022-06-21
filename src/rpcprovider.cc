#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"


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

//muduo库自调
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {
    if (!conn->connected()) {
        // 和rpc client的连接断开了
        conn->shutdown(); //close socket
    }
}

/*
在框架内部，RpcProvider和RpcConsumer要协商好通信用的protobuf数据格式，定义proto的message类型，进行数据的序列化和反序列化
需要：service_name method_name args
Tcp包格式：header_size(4B) + head_str(service_name + method_name + args_size) + args_str
- 传args_size是为了避免TCP黏包
- head_str用protobuf反序列化
*/
// 已建立连接用户的读写事件回调 如果远程有一个rpc服务的调用请求，那么OnMessage方法就会响应
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer, muduo::Timestamp) {
    // 接收远程rpc调用请求的字符流 Login args
    std::string recv_buf = buffer->retrieveAllAsString();

    //读header_size(注意这个header_size存储是int不是string)
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0); //拷4个字节出来

    //读header
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    //protobuf反序列化
    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcHeader.ParseFromString(rpc_header_str)) {
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    } else {
        std::cout << "rpc_header_str:" << rpc_header_str << " parse error!" << std::endl;
        return;
    }

    // 读args
    std::string args_str = recv_buf = recv_buf.substr(4 + header_size, args_size);

    //打印调试信息
    std::cout << "=========================================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << args_str << std::endl;
    std::cout << "=========================================================" << std::endl;


}