#include "log.h"
int main()
{
    // 设置日志级别为 DEBUG
    // ServerLogger::getInstance().setLogLevel(LogLevel::DEBUG);

    // 记录不同级别的日志,可以不同等级的记录日志
    LOG(LogLevel::INFO, "Application started.\n");
    LOG(LogLevel::WARNING, "Memory usage high.\n");
    LOG(LogLevel::ERROR, "File not found: ", "example.txt\n");
    // 示例：模拟循环记录日志，可以接收多个参数的日志
    for (int i = 0; i < 5; ++i)
    {
        LOG(LogLevel::DEBUG, "Debug message ", i,"\n");
    }

    return 0;
}