# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.14)

# 项目名称和版本
project(MyCppServerFramework VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含项目目录下的cmake子目录，以便使用自定义模块
# list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# 包含头文件目录
include_directories(include)

# 添加编译后的库文件目录到链接路径
link_directories(lib)

# 启用测试
# enable_testing()

# 添加子目录
# 例如，如果你的源代码在src目录下，测试代码在tests目录下
# add_subdirectory(src)
# add_subdirectory(tests)

# 查找第三方库
# find_package(<PackageName> REQUIRED)

# 设置可执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
# 定义可执行文件
add_executable(${PROJECT_NAME} src/main.cpp)

# 链接第三方库
# target_link_libraries(${PROJECT_NAME} <PackageName>)

# 安装规则
# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)


# 打印配置信息
# message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
# message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
