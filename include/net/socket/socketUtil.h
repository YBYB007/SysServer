#pragma once
#include "socket.h"
#include <memory>

class socketUtil
{
private:
    // 检测机制
    static bool isValidIP(const std::string &ip);
    static bool isValidPort(const std::string &port);

public:
    // 创建电话机
    static SOCKET getIPv4TCPSocket();
    static SOCKET getIPv6TCPSocket();
    static SOCKET getIPv4UDPSocket();
    static SOCKET getIPv6UDPSocket();

    // 制作地址
    static sockaddr_in makeSocketAddressWithIPv4(const char *IP, const char *port);
    static sockaddr_in6 makeSocketAddressWithIPv6(const char *IP, const char *port);

    // TCP套接字
    // 监听
    // int listen(int sock, int backlog);
    // 连接
    // int connect()
    // 接受
    // int accept(int sock, struct sockaddr * addr, socklen_t * addrlen)


    // 套接字选项设置
    // 地址复用
    static void setReUseSocketAddr(SOCKET fd);
};
