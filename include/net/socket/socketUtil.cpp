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

/*

void socketFuncs::SetNonBlock(SOCKET fd)
{
#if defined(__linux) || defined(__linux__)
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    unsigned long on = 1;
    ioctlsocket(fd, FIONBIO, &on);
#endif
}

void socketFuncs::SetBlock(SOCKET fd, int write_timeout)
{
#if defined(__linux) || defined(__linux__)
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags & (~O_NONBLOCK));
#elif defined(WIN32) || defined(_WIN32)
    unsigned long on = 0;
    ioctlsocket(fd, FIONBIO, &on);
#else
#endif
    if (write_timeout > 0)
    {
#ifdef SO_SNDTIMEO
#if defined(__linux) || defined(__linux__)
        struct timeval tv = {write_timeout / 1000, (write_timeout % 1000) * 1000};
        setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof tv);
#elif defined(WIN32) || defined(_WIN32)
        unsigned long ms = (unsigned long)write_timeout;
        setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&ms, sizeof(unsigned long));
#else
#endif
#endif
    }
}

void socketFuncs::SetReuseAddr(SOCKET sockfd)
{
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof on);
}

void socketFuncs::SetReusePort(SOCKET sockfd)
{
#ifdef SO_REUSEPORT
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const char *)&on, sizeof(on));
#endif
}

void socketFuncs::SetNoDelay(SOCKET sockfd)
{
#ifdef TCP_NODELAY
    int on = 1;
    int ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));
#endif
}

void socketFuncs::SetKeepAlive(SOCKET sockfd)
{
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));
}

void socketFuncs::SetNoSigpipe(SOCKET sockfd)
{
#ifdef SO_NOSIGPIPE
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, (char *)&on, sizeof(on));
#endif
}

void socketFuncs::SetSendBufSize(SOCKET sockfd, int size)
{
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&size, sizeof(size));
}

void socketFuncs::SetRecvBufSize(SOCKET sockfd, int size)
{
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&size, sizeof(size));
}

std::string socketFuncs::GetPeerIp(SOCKET sockfd)
{
    struct sockaddr_in addr = {0};
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr *)&addr, &addrlen) == 0)
    {
        return inet_ntoa(addr.sin_addr);
    }
    return "0.0.0.0";
}

std::string socketFuncs::GetSocketIp(SOCKET sockfd)
{
    struct sockaddr_in addr = {0};
    char str[INET_ADDRSTRLEN] = "127.0.0.1";
    if (GetSocketAddr(sockfd, &addr) == 0)
    {
        inet_ntop(AF_INET, &addr.sin_addr, str, sizeof(str));
    }
    return str;
}

int socketFuncs::GetSocketAddr(SOCKET sockfd, struct sockaddr_in *addr)
{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    return getsockname(sockfd, (struct sockaddr *)addr, &addrlen);
}

uint16_t socketFuncs::GetPeerPort(SOCKET sockfd)
{
    struct sockaddr_in addr = {0};
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr *)&addr, &addrlen) == 0)
    {
        return ntohs(addr.sin_port);
    }
    return 0;
}

int socketFuncs::GetPeerAddr(SOCKET sockfd, struct sockaddr_in *addr)
{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    return getpeername(sockfd, (struct sockaddr *)addr, &addrlen);
}
*/