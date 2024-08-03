#pragma once
#include "IPv6UdpSocket.h"

IPv6UdpSocket::IPv6UdpSocket()
{
    my_udp_socket = -1;
}
SOCKET IPv6UdpSocket::socketCreate()
{
    my_udp_socket = socketUtil::getIPv6UDPSocket();
    return my_udp_socket;
}
bool IPv6UdpSocket::socketBind(const char *IP, const char *port)
{
    sockaddr_in6 addr = socketUtil::makeSocketAddressWithIPv6(IP, port);
    if (::bind(my_udp_socket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}
void IPv6UdpSocket::socketClose()
{
    ::close(my_udp_socket);
    my_udp_socket = 0;
}
SOCKET IPv6UdpSocket::socketGet() const
{
    return my_udp_socket;
}
IPv6UdpSocket::~IPv6UdpSocket()
{
    if (my_udp_socket > 0)
    {
        this->socketClose();
    }
}