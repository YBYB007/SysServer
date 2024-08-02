#pragma once
#include "socketUtil.cpp"
class tcpSocket
{
protected:
    SOCKET my_tcp_socket;

public:
    virtual SOCKET socketCreate() = 0;
    virtual bool socketBind(const char *IP, const char *port) = 0;
    virtual bool socketListen(int backlog) = 0;
    virtual SOCKET socketAccept() = 0;
    virtual bool socketConnect(const char *IP, const char *port, int timeout) = 0;
    virtual void socketClose() = 0;
    virtual void socketShutdownWrite() = 0;
    virtual SOCKET socketGet() const = 0;
    virtual ~tcpSocket();
};
tcpSocket::~tcpSocket() {}
