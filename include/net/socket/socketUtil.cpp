#pragma once
#include "socketUtil.h"
#include <regex>
#include <iostream>
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

void socketUtil::setReUseSocketAddr(SOCKET fd)
{
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}