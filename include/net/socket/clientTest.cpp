#include <iostream>
#include "IPv4TcpSocket.cpp"
#include <memory>
int main(int argc, char const *argv[])
{
    /* code */
    IPv4TcpSocket client_sock;
    if(!client_sock.socketCreate()){
        std::cout<<"创建失败\n";
        exit(0);
    }
    if(!client_sock.socketConnect("192.168.12.128","12345",2)){
        std::cout<<"连接失败\n";
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

    return 0;
}
