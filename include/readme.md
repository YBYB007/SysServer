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


####   threadSafeDs 线程安全的数据结构




### network：





### protocol：HTTP 协议、