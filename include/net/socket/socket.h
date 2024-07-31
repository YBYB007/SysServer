#ifndef SOCKET_H
#define SOCKET_H
// 如果编译环境是 Linux
#if defined(__linux) || defined(__linux__)
/*
<sys/types.h>：基本系统数据类型定义。
<sys/socket.h>：套接字接口函数。
<sys/ioctl.h>：I/O 控制函数。
<netinet/in.h>：Internet 地址族。
<netinet/ether.h>：以太网协议定义。
<netinet/ip.h>：IP 协议定义。
<netpacket/packet.h>：网络数据包套接字接口。
<arpa/inet.h>：Internet 地址转换函数。
<net/ethernet.h>：Ethernet 协议定义。
<net/route.h>：路由表操作。
<netdb.h>：域名解析函数。
<net/if.h>：网络接口定义。
<unistd.h>：POSIX 操作系统 API。
<fcntl.h>：文件控制函数。
<errno.h>：错误码定义。
<sys/select.h>：select 函数定义。
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/route.h>
#include <netdb.h>
#include <net/if.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
/*
#define SOCKET int：将 SOCKET 定义为 int 类型，在 Linux 下套接字描述符是整数。
#define INVALID_SOCKET (-1)：定义无效的套接字描述符。
#define SOCKET_ERROR (-1)：定义套接字操作错误。
*/
#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

// 如果编译环境是 Windows
/*
#define FD_SETSIZE 1024：指定 FD_SETSIZE 的大小。
#define WIN32_LEAN_AND_MEAN：精简包含 Windows 头文件。
#define _WINSOCK_DEPRECATED_NO_WARNINGS：禁用某些已弃用的警告。
<WinSock2.h>：Windows 下套接字接口。
<windows.h>：Windows 操作系统 API。
<ws2tcpip.h>：TCP/IP 协议相关定义。
<iphlpapi.h>：IP 帮助 API。
*/
#elif defined(WIN32) || defined(_WIN32)
#define FD_SETSIZE 1024
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
/*
定义了关闭套接字的宏 SHUT_RD、SHUT_WR 和 SHUT_RDWR。
*/
#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2

#else

#endif
/*
<cstdint>：定义了标准整数类型。
<cstring>：定义了 C 字符串操作函数。
*/
#include <cstdint>
#include <cstring>

#endif