#pragma once
#include "socket.h"
#include <memory>
#include <vector>
#include <iostream>

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

    // 消息发送
    // 已知地址发送
    static int sendMessageByIPv4(SOCKET fd, const char *IP, const char *port, std::string message);
    static int sendMessageByIPv6(SOCKET fd, const char *IP, const char *port, std::string message);

    // 客户端地址接收
    static int recvMessageByIPv4(SOCKET fd, const char *IP, const char *port, std::vector<char> &buffer);
    static int recvMessageByIPv6(SOCKET fd, const char *IP, const char *port, std::vector<char> &buffer);

    // UDP echo服务程序
    static void udpEcho(SOCKET fd, std::vector<char> &buffer);

    // 已连接地址发送
    static int sendMessage(SOCKET fd, std::string message);
    // 已连接地址接收
    static int recvMessage(SOCKET fd, std::vector<char> &buffer);

    // 套接字选项设置
    // 地址复用,断开后可以直接重新绑定
    static void setReUseSocketAddr(SOCKET fd);
    static void SetReusePort(SOCKET fd);
    // 设置缓冲区大小
    static void SetSendBufSize(SOCKET fd, int size);
    static void SetRecvBufSize(SOCKET fd, int size);
    // 阻塞与非阻塞
    static void socketNoBlock(SOCKET fd);
    static void socketBlock(SOCKET fd, int write_timeout);
    // TCP noNagle算法
    static void noNage(SOCKET fd);
    // 保持存活
    static void setKeepAlive(SOCKET fd);
    // 无信息终止
    static void setNoSigpipe(SOCKET fd);
    // 套接字地址获取

    // 获取对方IP用IPv4的格式
    static std::string getPeerIpv4(SOCKET fd);
    // 获取对方端口以IPv4的格式
    static std::string getPeerIPv4Port(SOCKET fd);
    // 获取对方IP用IPv6的格式
    static std::string getPeerIpv6(SOCKET fd);
    // 获取对方IP以IPv6的格式
    static std::string getPeerIPv6Port(SOCKET fd);
    // 获取Socket的IP以IPv4的格式
    static std::string getSocketIpv4(SOCKET fd);
    // 获取Socket的IP以IPv4的格式
    static std::string getSocketIpv6(SOCKET fd);
    // 获取Socket的地址以IPv4的格式
    static sockaddr_in* getSocketAddrIPv4(SOCKET fd);
    // 获取对方地址以IPv4的格式
    static sockaddr_in* getPeerAddrIPv4(SOCKET fd);
    // 获取Socket的地址以IP6的格式
    static sockaddr_in6* getSocketAddrIPv6(SOCKET fd);
    // 获取对方地址以IPv6的格式
    static sockaddr_in6* getPeerAddrIPv6(SOCKET fd);

    static std::string sockaddr_to_string(const sockaddr *addr);

};
