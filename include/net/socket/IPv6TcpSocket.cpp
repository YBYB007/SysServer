#pragma once
#include "IPv6TcpSocket.h"

IPv6TcpSocket::IPv6TcpSocket(/* args */)
{
    my_tcp_socket = -1;
}
void IPv6TcpSocket::setIPv6TcpSocket(SOCKET fd)
{
    my_tcp_socket = fd;
}
SOCKET IPv6TcpSocket::socketCreate()
{
    my_tcp_socket = socketUtil::getIPv6TCPSocket();
    return my_tcp_socket;
}
bool IPv6TcpSocket::socketBind(const char *IP, const char *port)
{
    sockaddr_in6 addr = socketUtil::makeSocketAddressWithIPv6(IP, port);
    if (::bind(my_tcp_socket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}
bool IPv6TcpSocket::socketListen(int backlog)
{
    if (::listen(my_tcp_socket, backlog) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}
SOCKET IPv6TcpSocket::socketAccept()
{
    sockaddr_in6 addr = {0};
    socklen_t addrlen = sizeof addr;
    SOCKET accept_socket_fd = ::accept(my_tcp_socket, (struct sockaddr *)&addr, &addrlen);
    return accept_socket_fd;
}
bool IPv6TcpSocket::socketConnect(const char *IP, const char *port, int timeout = 0)
{
    sockaddr_in6 server_address = socketUtil::makeSocketAddressWithIPv6(IP, port);
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
void IPv6TcpSocket::socketClose()
{
    ::close(my_tcp_socket);
    my_tcp_socket = 0;
}
void IPv6TcpSocket::socketShutdownWrite()
{
    shutdown(my_tcp_socket, SHUT_WR);
    my_tcp_socket = 0;
}
SOCKET IPv6TcpSocket::socketGet() const
{
    return my_tcp_socket;
}
IPv6TcpSocket::~IPv6TcpSocket()
{
    if (my_tcp_socket > 0)
    {
        this->socketClose();
    }
}