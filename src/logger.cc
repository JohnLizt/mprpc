#include "logger.h"

// 获取日志的单例
Logger& Logger::GetInstance() {
    static Logger logger;
    return logger;
}

Logger::Logger() {
    // 启动专门的写日志线程(从缓冲区取日志写入文件)
    std::thread writeLogTask([&](){
        for (;;) {
            // 获取当前的日期，取日志信息，写入相应的日志文件当中
            time_t now = time(nullptr);
            tm* nowtm = localtime(&now);

            char file_name[123];
            sprintf(file_name, "%d-%d-%d-log.txt", nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);

            FILE* pf = fopen(file_name, "a+");
            if (pf == nullptr) {
                std::cout << "logger file :" << file_name << "open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = m_lckQue.Pop();
            fputs(msg.c_str(), pf);
            fclose(pf);
        }
    });
    // 设置分离线程，守护线程
    writeLogTask.detach();
}

// 设置日志级别
void SetLogLevel(LogLevel level) {
    m_logLevel = level;
}
// 写日志(lockqueue缓冲区)，由外部调用
void Log(std::string msg) {
    m_lckQue.Push(msg);
}


