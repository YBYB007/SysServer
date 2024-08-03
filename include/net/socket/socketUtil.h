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
    static void udpEcho(SOCKET fd,std::vector<char> &buffer);


    // 已连接地址发送
    static int sendMessage(SOCKET fd, std::string message);
    // 已连接地址接收
    static int recvMessage(SOCKET fd, std::vector<char> &buffer);

    // 套接字选项设置
    // 地址复用,断开后可以直接重新绑定
    static void setReUseSocketAddr(SOCKET fd);

    /*

static void SetNonBlock(SOCKET fd);
static void SetBlock(SOCKET fd, int write_timeout = 0);
static void SetReuseAddr(SOCKET fd);
static void SetReusePort(SOCKET sockfd);
static void SetNoDelay(SOCKET sockfd);
static void SetKeepAlive(SOCKET sockfd);
static void SetNoSigpipe(SOCKET sockfd);
static void SetSendBufSize(SOCKET sockfd, int size);
static void SetRecvBufSize(SOCKET sockfd, int size);
static std::string GetPeerIp(SOCKET sockfd);
static std::string GetSocketIp(SOCKET sockfd);
static int GetSocketAddr(SOCKET sockfd, struct sockaddr_in *addr);
static uint16_t GetPeerPort(SOCKET sockfd);
static int GetPeerAddr(SOCKET sockfd, struct sockaddr_in *addr);
    */
};
