#pragma once
#include "udpSocket.h"

class IPv6UdpSocket : public udpSocket
{
public:
    IPv6UdpSocket();
    void setIPv6TcpSocket(SOCKET fd);
    virtual SOCKET socketCreate() override;
    virtual bool socketBind(const char *IP, const char *port) override;
    virtual void socketClose() override;
    virtual SOCKET socketGet() const override;
    ~IPv6UdpSocket();
};
