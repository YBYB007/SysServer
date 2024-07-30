# 头文件



### syswork：

####   threadPool 线程池

​      配置CMakeLists.txt

      ```cmake
find_package(Threads REQUIRED)

add_executable(my_executable src/main.cpp)

target_link_libraries(my_executable
	PRIVATE
	Threads::Threads
)
      ```





#### memoryPool 内存池

​	线程分离的高并发内存池



### network：





### protocol：HTTP 协议、