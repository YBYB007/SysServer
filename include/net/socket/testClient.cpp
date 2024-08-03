#include <iostream>
#include "IPv4TcpSocket.cpp"
#include "IPv4UpdSocket.cpp"
#include <memory>

void testUdpClient()
{
    IPv4UdpSocket udp_sock;
    udp_sock.socketCreate();
    if (!udp_sock.socketCreate())
    {
        std::cout << "创建失败\n";
        exit(0);
    }

    if (socketUtil::sendMessageByIPv4(udp_sock.socketGet(), "192.168.12.128", "12347", std::move("Hello, server!")) < 0)
    {
        std::cout << "发送错误" << std::endl;
    }
    std::cout << "Message sent. Waiting for echo..." << std::endl;

    std::vector<char> buffer(1024); // 数据接收缓冲区

    if (socketUtil::recvMessage(udp_sock.socketGet(), buffer) < 0)
    {
        std::cout << "没收到消息" << std::endl;
    }
    std::cout << "Received: " << std::string(&buffer[0], buffer.size()) << std::endl;

    udp_sock.socketClose();
}

void testTcpClient()
{
    IPv4TcpSocket client_sock;
    if (!client_sock.socketCreate())
    {
        std::cout << "创建失败\n";
        exit(0);
    }
    if (!client_sock.socketConnect("192.168.12.128", "12345", 2))
    {
        std::cout << "连接失败\n";
        exit(0);
    }

    std::cout << "连接成功" << std::endl;

    std::string messages = "hello server!";
    // 发送消息到服务器

    if (send(client_sock.socketGet(), messages.c_str(), strlen(messages.c_str()) + 1, 0) < 0)
    {
        perror("send failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "信息发送" << std::endl;

    // 接收消息
    char buffer[1024] = {0}; // 数据接收缓冲区
    read(client_sock.socketGet(), buffer, 1024);
    std::cout << "接收信息: " << buffer << std::endl;

    client_sock.socketClose();
}

int main(int argc, char const *argv[])
{
    /* code */
    // testTcpClient();
    // testUdpClient();
    return 0;
}
