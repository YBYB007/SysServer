#pragma once
#include "socketUtil.h"
#include <regex>

SOCKET socketUtil::getIPv4TCPSocket()
{
    // int socket(int domain, int type, int protocol);

    return socket(AF_INET, SOCK_STREAM, 0);
}
SOCKET socketUtil::getIPv6TCPSocket()
{
    // int socket(int domain, int type, int protocol);
    return socket(AF_INET6, SOCK_STREAM, 0);
}
SOCKET socketUtil::getIPv4UDPSocket()
{
    // int socket(int domain, int type, int protocol);
    return socket(AF_INET, SOCK_DGRAM, 0);
}
SOCKET socketUtil::getIPv6UDPSocket()
{
    // int socket(int domain, int type, int protocol);
    return socket(AF_INET6, SOCK_DGRAM, 0);
}

bool socketUtil::isValidIP(const std::string &ip)
{
    std::regex ipv4_regex("^\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b$");
    std::regex ipv6_regex("^\\b(?:[0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}\\b$");
    return std::regex_match(ip, ipv4_regex) || std::regex_match(ip, ipv6_regex);
}
bool socketUtil::isValidPort(const std::string &port)
{
    try
    {
        int portNum = std::stoi(port);
        return portNum >= 0 && portNum <= 65535;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

sockaddr_in socketUtil::makeSocketAddressWithIPv4(const char *IP = INADDR_ANY, const char *port = "8080")
{
    if (isValidIP(IP) && isValidPort(port))
    {
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;              // IPv4地址族
        server_address.sin_port = htons(std::stoi(port)); // 设置端口号，注意使用htons函数将主机字节序转换为网络字节序
        server_address.sin_addr.s_addr = inet_addr(IP);   // 设置IP地址，使用inet_addr函数将字符串形式的IP地址转换为二进制形式
        return std::move(server_address);
    }
    else
    {
        perror("socket address invalid");
        exit(EXIT_FAILURE);
    }
}

sockaddr_in6 socketUtil::makeSocketAddressWithIPv6(const char *IP = "::1", const char *port = "8080")
{
    sockaddr_in6 addr;
    if (isValidIP(IP) && isValidPort(port))
    {
        memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;

        inet_pton(AF_INET6, IP, &(addr.sin6_addr));
        addr.sin6_port = htons(std::stoi(port));

        return addr;
    }
    else
    {
        perror("socket address invalid");
        exit(EXIT_FAILURE);
    }
}

// 消息发送
// 已知地址发送
int socketUtil::sendMessageByIPv4(SOCKET fd, const char *IP, const char *port, std::string message)
{
    sockaddr_in servaddr = socketUtil::makeSocketAddressWithIPv4(IP, port); // 服务器地址结构
    return sendto(fd, message.c_str(), message.size(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
}

int socketUtil::sendMessageByIPv6(SOCKET fd, const char *IP, const char *port, std::string message)
{
    sockaddr_in6 servaddr = socketUtil::makeSocketAddressWithIPv6(IP, port); // 服务器地址结构
    return sendto(fd, message.c_str(), message.size(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
}

// 客户端地址接收
int socketUtil::recvMessageByIPv4(SOCKET fd, const char *IP, const char *port, std::vector<char> &buffer)
{
    sockaddr_in servaddr = socketUtil::makeSocketAddressWithIPv4(IP, port);
    socklen_t len = sizeof(servaddr);
    return recvfrom(fd, &buffer[0], buffer.size(), 0, (struct sockaddr *)&servaddr, &len);
}
int socketUtil::recvMessageByIPv6(SOCKET fd, const char *IP, const char *port, std::vector<char> &buffer)
{
    sockaddr_in6 servaddr = socketUtil::makeSocketAddressWithIPv6(IP, port);
    socklen_t len = sizeof(servaddr);
    return recvfrom(fd, &buffer[0], buffer.size(), 0, (struct sockaddr *)&servaddr, &len);
}

void socketUtil::udpEcho(SOCKET fd, std::vector<char> &buffer)
{
    sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr); // 地址长度
                                     // 清空缓冲区
    // 接收客户端消息
    if (recvfrom(fd, &buffer[0], buffer.capacity(), 0, (struct sockaddr *)&cliaddr, &len) < 0)
    {
        std::cout << "没消息" << std::endl;
    }
    std::cout << "Received: " << std::string(&buffer[0], buffer.size()) << std::endl;
    // 发送回客户端消息
    if (sendto(fd, &buffer[0], buffer.size(), 0, (struct sockaddr *)&cliaddr, len) < 0)
    {
        std::cout << "发送失败" << std::endl;
    }
}

// 已知地址发送
int socketUtil::sendMessage(SOCKET fd, std::string message)
{
    return send(fd, message.c_str(), message.size(), 0);
}
// 已知地址接收
int socketUtil::recvMessage(SOCKET fd, std::vector<char> &buffer)
{
    return recv(fd, &buffer[0], buffer.size(), 0);
}

void socketUtil::setReUseSocketAddr(SOCKET fd)
{
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}

void socketUtil::SetReusePort(SOCKET fd)
{
#ifdef SO_REUSEPORT
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (const char *)&on, sizeof(on));
#endif
}

void socketUtil::SetSendBufSize(SOCKET fd, int size)
{
    setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&size, sizeof(size));
}

void socketUtil::SetRecvBufSize(SOCKET fd, int size)
{
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *)&size, sizeof(size));
}

void socketUtil::socketNoBlock(SOCKET fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void socketUtil::socketBlock(SOCKET fd, int write_timeout)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags & (~O_NONBLOCK));
    if (write_timeout > 0)
    {
        struct timeval tv = {write_timeout / 1000, (write_timeout % 1000) * 1000};
        setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof tv);
    }
}

void socketUtil::noNage(SOCKET fd)
{
#ifdef TCP_NODELAY
    int on = 1;
    int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));
#endif
}

void socketUtil::setKeepAlive(SOCKET fd)
{
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));
}

void socketUtil::setNoSigpipe(SOCKET fd)
{
    int on = 1;
    setsockopt(fd, SOL_SOCKET, MSG_NOSIGNAL, (char *)&on, sizeof(on));
}

std::string socketUtil::sockaddr_to_string(const sockaddr *addr)
{
    char addr_str[INET6_ADDRSTRLEN];
    if (addr->sa_family == AF_INET)
    {
        inet_ntop(AF_INET, &(((sockaddr_in *)addr)->sin_addr), addr_str, sizeof(addr_str));
    }
    else if (addr->sa_family == AF_INET6)
    {
        inet_ntop(AF_INET6, &(((sockaddr_in6 *)addr)->sin6_addr), addr_str, sizeof(addr_str));
    }
    return std::string(addr_str);
}

// 获取对方IPv4地址
std::string socketUtil::getPeerIpv4(SOCKET fd)
{
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getpeername(fd, (sockaddr *)&addr, &len) == 0)
    {
        return sockaddr_to_string((sockaddr *)&addr);
    }
    return "Error";
}

// 获取对方IPv4端口
std::string socketUtil::getPeerIPv4Port(SOCKET fd)
{
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getpeername(fd, (sockaddr *)&addr, &len) == 0)
    {
        return std::to_string(ntohs(addr.sin_port));
    }
    return "Error";
}

// 获取对方IPv6地址
std::string socketUtil::getPeerIpv6(SOCKET fd)
{
    sockaddr_in6 addr;
    socklen_t len = sizeof(addr);
    if (getpeername(fd, (sockaddr *)&addr, &len) == 0)
    {
        return sockaddr_to_string((sockaddr *)&addr);
    }
    return "Error";
}

// 获取对方IPv6端口
std::string socketUtil::getPeerIPv6Port(SOCKET fd)
{
    sockaddr_in6 addr;
    socklen_t len = sizeof(addr);
    if (getpeername(fd, (sockaddr *)&addr, &len) == 0)
    {
        return std::to_string(ntohs(addr.sin6_port));
    }
    return "Error";
}

// 获取Socket的IPv4地址
std::string socketUtil::getSocketIpv4(SOCKET fd)
{
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getsockname(fd, (sockaddr *)&addr, &len) == 0)
    {
        return sockaddr_to_string((sockaddr *)&addr);
    }
    return "Error";
}

// 获取Socket的IPv6地址
std::string socketUtil::getSocketIpv6(SOCKET fd)
{
    sockaddr_in6 addr;
    socklen_t len = sizeof(addr);
    if (getsockname(fd, (sockaddr *)&addr, &len) == 0)
    {
        return sockaddr_to_string((sockaddr *)&addr);
    }
    return "Error";
}

// 获取Socket的地址结构体IPv4
sockaddr_in *socketUtil::getSocketAddrIPv4(SOCKET fd)
{
    sockaddr_in *addr = new sockaddr_in;
    socklen_t len = sizeof(*addr);
    if (getsockname(fd, (sockaddr *)addr, &len) == 0)
    {
        return addr;
    }
    delete addr;
    return nullptr;
}

// 获取对方地址结构体IPv4
sockaddr_in *socketUtil::getPeerAddrIPv4(SOCKET fd)
{
    sockaddr_in *addr = new sockaddr_in;
    socklen_t len = sizeof(*addr);
    if (getpeername(fd, (sockaddr *)addr, &len) == 0)
    {
        return addr;
    }
    delete addr;
    return nullptr;
}

// 获取Socket的地址结构体IPv6
sockaddr_in6 *socketUtil::getSocketAddrIPv6(SOCKET fd)
{
    sockaddr_in6 *addr = new sockaddr_in6;
    socklen_t len = sizeof(*addr);
    if (getsockname(fd, (sockaddr *)addr, &len) == 0)
    {
        return addr;
    }
    delete addr;
    return nullptr;
}

// 获取对方地址结构体IPv6
sockaddr_in6 *socketUtil::getPeerAddrIPv6(SOCKET fd)
{
    sockaddr_in6 *addr = new sockaddr_in6;
    socklen_t len = sizeof(*addr);
    if (getpeername(fd, (sockaddr *)addr, &len) == 0)
    {
        return addr;
    }
    delete addr;
    return nullptr;
}