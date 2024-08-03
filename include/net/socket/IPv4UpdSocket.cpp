#pragma once
#include "IPv4UdpSocket.h"

IPv4UdpSocket::IPv4UdpSocket()
{
    my_udp_socket = -1;
}
SOCKET IPv4UdpSocket::socketCreate()
{
    my_udp_socket = socketUtil::getIPv4UDPSocket();
    return my_udp_socket;
}
bool IPv4UdpSocket::socketBind(const char *IP, const char *port)
{
    sockaddr_in addr = socketUtil::makeSocketAddressWithIPv4(IP, port);
    if (::bind(my_udp_socket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}
void IPv4UdpSocket::socketClose()
{
    ::close(my_udp_socket);
    my_udp_socket = 0;
}
SOCKET IPv4UdpSocket::socketGet() const
{
    return my_udp_socket;
}
IPv4UdpSocket::~IPv4UdpSocket()
{
    if (my_udp_socket > 0)
    {
        this->socketClose();
    }
}