#include <iostream>
#include "IPv4TcpSocket.cpp"
#include "IPv4UpdSocket.cpp"
#include <memory>

void testUdpServer()
{
    IPv4UdpSocket udp_sock;
    if (!udp_sock.socketCreate())
    {
        std::cout << "创建失败\n";
        exit(0);
    }

    if (!udp_sock.socketBind("192.168.12.128", "12347"))
    {
        std::cout << "绑定失败\n";
        exit(0);
    }
    std::cout << "UDP echo server is running on port " << std::endl;

    std::vector<char> buffer(1024); // 数据接收缓冲区
    for (;;)
    {
        socketUtil::udpEcho(udp_sock.socketGet(),buffer);
    }

    udp_sock.socketClose();
}

void testTcpServer()
{
    IPv4TcpSocket server_sock;
    if (!server_sock.socketCreate())
    {
        std::cout << "创建失败\n";
        exit(0);
    }

    if (!server_sock.socketBind("192.168.12.128", "12345"))
    {
        std::cout << "绑定失败\n";
        exit(0);
    }
    if (!server_sock.socketListen(3))
    {
        std::cout << "无法监听\n";
        exit(0);
    }
    IPv4TcpSocket data_sock;
    while (true)
    {
        data_sock.setIPv4TcpSocket(server_sock.socketAccept());

        std::cout << "连接成功!" << std::endl;

        char buffer[1024] = {0}; // 数据接收缓冲区

        // 接收消息
        read(data_sock.socketGet(), buffer, 1024);
        std::cout << "接收信息： " << buffer << std::endl;

        std::string str(buffer);
        // 发送响应消息
        const char *message = buffer;
        write(data_sock.socketGet(), message, strlen(message) + 1);
    }

    server_sock.socketClose();
    data_sock.socketClose();
}

int main(int argc, char const *argv[])
{
    /* code */
    // testTcpServer();
    // testUdpServer();
    return 0;
}
