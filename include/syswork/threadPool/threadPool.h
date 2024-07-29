#pragma once
// 线程组
#include <thread>
#include <vector>

// 任务队列
#include <queue>
#include <memory>
#include <functional>

// 并发编程
#include <mutex>
#include <condition_variable>
#include <future>

// 异常处理
#include <stdexcept>

class ThreadPool
{
private:
    std::vector<std::thread> workers;        // 存储工作线程的容器
    std::queue<std::function<void()>> tasks; // 任务队列

    std::mutex queue_mutex;            // 任务队列互斥量
    std::condition_variable condition; // 条件变量用于线程同步

    bool stop; // 标识线程池是否停止的布尔值
public:
    ThreadPool(size_t);
    //F 作为函数对象，Args 作为参数包
    template <class F, class... Args>                   // typename 类型名
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>; // 添加任务到线程池并返回 future 对象
    ~ThreadPool();                                                                                 
};