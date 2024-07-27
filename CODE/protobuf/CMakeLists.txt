cmake_minimum_required(VERSION 3.14)
project(MyProject)

# 查找 Protobuf 库
find_package(Protobuf REQUIRED)

# 包含 Protobuf 头文件
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# 定义 Protobuf 文件
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS person.proto)

# 添加可执行文件
add_executable(my_executable main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# 链接 Protobuf 库
target_link_libraries(my_executable ${Protobuf_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
