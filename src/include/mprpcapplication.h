#pragma once

// mprpc框架的基础类 负责框架初始化操作
class MprpcApplication
{
public:
    static void Init(int argc, char **argv);
    static MprpcApplication& GetInstance();
private:
    MprpcApplication() {}
    MprpcApplication(const MprpcApplication&) = delete; // 禁止拷贝、移动构造函数调用
    MprpcApplication(MprpcApplication&&) = delete;
};
