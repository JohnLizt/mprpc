#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>

void ShowArgsHelp() {
    std::cout << "format: command -i <configfile> " << std::endl;
}

void MprpcApplication::Init(int argc, char **argv) { // 外部实现static方法不用加static
    if (argc < 2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
    
    int c = 0;
    std::string config_file;
    while ((c = getopt(argc,argv,"i:")) != -1) {
        switch (c) {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                std::cout << "invalid args!" << std::endl;
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            case ':':
                std::cout << "need <configfile>!" << std::endl;
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    // 开始加载配置文件 rpcserver_ip= rpcserver_port= zookeeper_ip= zookeep_port=


}

MprpcApplication& MprpcApplication::GetInstance() {
    static MprpcApplication app;
    return app;
}

