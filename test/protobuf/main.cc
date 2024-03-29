#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main() {
    LoginResponse rsp;
    ResultCode *rc = rsp.mutable_result(); // message里套message的操作方式
    rc->set_errcode(1);
    rc->set_errmsg("登陆处理失败了");

    GetFriendListResponse rsp1;
    ResultCode *rc = rsp1.mutable_result();
    rc->set_errcode(0);

    //列表对象怎么操作
    User *user1 = rsp.add_friend_list();
    user1->set_name("zhang san");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    User *user2 = rsp.add_friend_list();
    user2->set_name("li si");
    user2->set_age(22);
    user2->set_sex(User::MAN);

    std::cout << rsp.friend_list_size() << std::endl;
    return 0;
}

int main1() {
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    // 序列化：对象 - string
    std::string send_str;
    if(req.SerializeToString(&send_str)){
        std::cout << send_str.c_str() << std::endl;
    }

    // 反序列化：string - 对象
    LoginRequest reqB;
    if (reqB.ParseFromString(send_str)) {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }

    return 0;
}