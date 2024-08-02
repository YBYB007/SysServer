#pragma once 

// 系统结果
#include <sys/types.h> // 基本系统数据类型定义
#include <sys/socket.h> // 套接字接口函数
#include <sys/ioctl.h> // I/O 控制函数
#include <unistd.h> // POSIX 操作系统 API
#include <sys/select.h> // select 函数定义
#include <fcntl.h> // 文件控制函数
#include <error.h> // 错误码控制函数

// 网络协议
#include <netinet/in.h> // 因特网地址簇
#include <netinet/ether.h> // 以太网协议
#include <net/ethernet.h> // 以太网协议定义
#include <netinet/ip.h> //IP协议
#include <netpacket/packet.h> // 网络数据包套接字接口
#include <net/route.h> //路由表解析协议
#include <netdb.h> // 域名解析函数
#include <arpa/inet.h> // 地址转换函数
#include <net/if.h> // 网络接口定义

// 套接字行为定义
#define SOCKET int // 在 Linux 下套接字描述符是整数。
#define INVALID_SOCKET (-1) // Linux下文件描述符是从0开始的 -1代表着无效的套接字描述
#define SOCKET_ERROR (-1) // 套接字操作错误

// 字符串
#include <cstdint>
#include <cstring>

