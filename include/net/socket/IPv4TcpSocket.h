#pragma once
#include "tcpSocket.h"

class IPv4TcpSocket : public tcpSocket
{
public:
    IPv4TcpSocket();
    void setIPv4TcpSocket(SOCKET fd);
    virtual SOCKET socketCreate() override;
    virtual bool socketBind(const char *IP, const char *port) override;
    virtual bool socketListen(int backlog) override;
    virtual SOCKET socketAccept() override;
    virtual bool socketConnect(const char *IP, const char *port, int timeout) override;
    virtual void socketClose() override;
    virtual void socketShutdownWrite() override;
    virtual SOCKET socketGet() const override;
    virtual ~IPv4TcpSocket() override;
};
