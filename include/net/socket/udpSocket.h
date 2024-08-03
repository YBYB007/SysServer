#pragma once
#include "socketUtil.cpp"
class udpSocket
{
protected:
    SOCKET my_udp_socket;
public:
    virtual SOCKET socketCreate() = 0;
    virtual bool socketBind(const char *IP, const char *port) = 0;
    virtual void socketClose() = 0;
    virtual SOCKET socketGet() const = 0;
    virtual ~udpSocket();
};
udpSocket::~udpSocket(){}
