# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.14)

# 项目名称和版本
project(MyCppServerFramework VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(testServer testServer.cpp)
add_executable(testClient testClient.cpp)

