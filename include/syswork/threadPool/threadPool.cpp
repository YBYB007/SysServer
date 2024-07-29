#pragma once
#include "threadPool.h"
// 线程池构造函数定义，启动指定数量的工作线程
ThreadPool::ThreadPool(size_t threads) : stop(false) // 初始化 stop 为 false
{
    for (size_t i = 0; i < threads; ++i)
    {
        // 工作线程压入
        workers.emplace_back(
            [this]
            {
                // 循环接任务
                for (;;)
                {
                    std::function<void()> task;

                    // 这里是为了锁的释放
                    {
                        // 给任务队列上锁 只有一个线程拿到锁
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        // 如果任务队列不是空的，或者线程池没有停止运行，就在这里等任务，释放锁,等到通知来了再锁上
                        this->condition.wait(lock, [this]
                                             { return this->stop || !this->tasks.empty(); });
                        // 如果任务队列没有了，或者线程池停止运行，就停止线程。
                        if (this->stop && this->tasks.empty())
                            return;
                        // 接到后任务打包给线程
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    // 线程执行任务
                    task();
                }
            });
    }
}

// 将新任务添加到线程池中
template <class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    // 拿到执行函数的返回类型 F(Args...)
    using return_type = typename std::result_of<F(Args...)>::type;

    // task包裹智能指针
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    // 任务执行的结果保存到res，类型为Future
    std::future<return_type> res = task->get_future();

    // 这里是为了解锁
    {
        // 锁队列
        std::unique_lock<std::mutex> lock(queue_mutex);
        // 如果停止运行了，就报错，这并不是一个正在运行的线程池
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        // 将任务的执行函数压入队列
        tasks.emplace(
            // 捕获这个包含执行体的任务，把执行体放在里面，拿到任务的线程在调用执行的时候就执行这个函数体
            [task]()
            { (*task)(); });
    }
    // 唤醒一个线程，让他过来接任务
    condition.notify_one();
    // 然后把最后保存结果的对象返回，等待未来某一时刻需要时使用
    return res;
}

// 线程池析构函数定义，等待所有线程执行完毕后退出
inline ThreadPool::~ThreadPool()
{
    // 这里是为了解锁
    {
        // 锁住不让任何线程再接任务
        std::unique_lock<std::mutex> lock(queue_mutex);
        // 将线程池停止
        stop = true;
    }
    // 通知所有没接到任务的线程结束
    condition.notify_all();

    // 等待所有运行的线程运行完
    for (std::thread &worker : workers)
        worker.join();
}