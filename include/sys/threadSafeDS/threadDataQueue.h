#pragma once
#include <queue>
#include <future>
#include <mutex>
#include <condition_variable>

// 数据队列
template <class dataType>
class threadDataQueue
{
private:
    /* data */
    std::queue<dataType> dataQueue;
    std::mutex queueMutex;
    std::condition_variable queueWait;

public:
    threadDataQueue(/* args */);
    ~threadDataQueue();
    bool isEmpty();
    void push(dataType data);
    void wake();
    void clear();
    size_t size() const;
    bool waitPop(dataType &value);
    bool tyrPop(dataType &value);
    std::shared_ptr<dataType> tryPop();
};
