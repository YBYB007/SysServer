#pragma once
#include "IPv4TcpSocket.h"
IPv4TcpSocket::IPv4TcpSocket()
{
    my_tcp_socket = -1;
}
void IPv4TcpSocket::setIPv4TcpSocket(SOCKET fd)
{
    my_tcp_socket = fd;
}
SOCKET IPv4TcpSocket::socketCreate()
{
    my_tcp_socket = socketUtil::getIPv4TCPSocket();
    return my_tcp_socket;
}
bool IPv4TcpSocket::socketBind(const char *IP, const char *port)
{
    sockaddr_in addr = socketUtil::makeSocketAddressWithIPv4(IP, port);
    if (::bind(my_tcp_socket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}
bool IPv4TcpSocket::socketListen(int backlog)
{
    if (::listen(my_tcp_socket, backlog) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}
SOCKET IPv4TcpSocket::socketAccept()
{
    sockaddr_in addr = {0};
    socklen_t addrlen = sizeof addr;
    SOCKET accept_socket_fd = ::accept(my_tcp_socket, (struct sockaddr *)&addr, &addrlen);
    return accept_socket_fd;
}
bool IPv4TcpSocket::socketConnect(const char *IP, const char *port, int timeout = 0)
{
    sockaddr_in server_address = socketUtil::makeSocketAddressWithIPv4(IP, port);
    // 设置超时时间
    if (timeout > 0)
    {
        struct timeval tv;
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        setsockopt(my_tcp_socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof tv);
    }
    if (::connect(my_tcp_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}
void IPv4TcpSocket::socketClose()
{
    ::close(my_tcp_socket);
    my_tcp_socket = 0;
}
void IPv4TcpSocket::socketShutdownWrite()
{
    shutdown(my_tcp_socket, SHUT_WR);
    my_tcp_socket = 0;
}
SOCKET IPv4TcpSocket::socketGet() const
{
    return my_tcp_socket;
}
IPv4TcpSocket::~IPv4TcpSocket()
{
    if (my_tcp_socket > 0)
    {
        this->socketClose();
    }
}