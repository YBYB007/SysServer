#pragma once
#include "udpSocket.h"
class IPv4UdpSocket : public udpSocket
{
public:
    IPv4UdpSocket();
    void setIPv4UdpSocket(SOCKET fd);
    virtual SOCKET socketCreate() override;
    virtual bool socketBind(const char *IP, const char *port) override;
    virtual void socketClose() override;
    virtual SOCKET socketGet() const override;
    ~IPv4UdpSocket();
};
