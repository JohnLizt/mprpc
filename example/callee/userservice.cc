# include <iostream>
# include <string>
# include "user.pb.h"
# include "mprpcapplication.h"
# include "rpcprovider.h"
# include "logger.h"

/*
    UserService原先是本地服务，提供两个本地方法Login和GetFriendLists
*/
class UserService : public fixbug::UserServiceRpc { // 使用在rpc服务提供方的
public:
    bool Login(std::string name, std::string pwd) {
    std::cout << "doing local service: Login" << std::endl;
    std::cout << "name:" << name << "pwd:" << pwd << std::endl;
    return true;
    }

    // 重写基类UserServiceRpc的虚函数 下面这些方法都是框架自动调用的
    //1. caller ===> Login(LoginRequest) => muduo => callee
    //2. callee ===> Login(LoginRequest) => 交到下面重写的Login方法了
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done) 
    {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name(); //直接获得反序列化好的数据，不需自己解析
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        //执行回调操作 执行响应对象数据的序列化和网络发送（都是由框架完成的）
        done->Run();
    }
};

int main (int argc, char** argv) {
    // 框架初始化 对应基础类 Q：Init是不需要定义的吗？A:需要
    MprpcApplication::Init(argc, argv); // 读配置文件、ip端口号、日志路径等等
    LOG_INFO("first log message!");
    LOG_ERR("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);

    // 把UserService对象发布(注册)到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点 Run以后，进程进入阻塞状态，等待远程rpc调用请求
    provider.Run();
    return 0;
}