#pragma once
#include "threadDataQueue.h"

template <class dataType>
threadDataQueue<dataType>::threadDataQueue(/* args */)
{
}

template <class dataType>
threadDataQueue<dataType>::~threadDataQueue()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        std::queue<datatype> emptyQueue;
        dataQueue.swap(emptyQueue);
        queueWait.notify_all();
    }
}

template <class dataType>
bool threadDataQueue<dataType>::isEmpty()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        return dataQueue.empty();
    }
}

template <class dataType>
void threadDataQueue<dataType>::push(dataType data)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        dataQueue.push(data);
    }
}

template <class dataType>
void threadDataQueue<dataType>::wake()
{
    queueWait.notify_one();
}

template <class dataType>
void threadDataQueue<dataType>::clear()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        std::queue<datatype> emptyQueue;
        dataQueue.swap(emptyQueue);
    }
}

template <class dataType>
size_t threadDataQueue<dataType>::size() const
{
    std::lock_guard<std::mutex> lock(_mutex); // 锁定本队列的互斥量
    return _dataQueue.size();                 // 返回队列的大小
}

template <class dataType>
bool threadDataQueue<dataType>::waitPop(dataType &value)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueWait.wait(lock);
        if (this->dataQueue.empty())
        {
            return false;
        }
        value = dataQueue.front();
        dataQueue.pop();
        return true;
    }
}

template <class dataType>
bool threadDataQueue<dataType>::tyrPop(dataType &value)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (this->dataQueue.empty())
        {
            return false;
        }
        value = dataQueue.front();
        dataQueue.pop();
        return true;
    }
}

template <class dataType>
std::shared_ptr<dataType> threadDataQueue<dataType>::tryPop()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex); // 锁定本队列的互斥量
        if (dataQueue.empty())
            return std::shared_ptr<T>();

        std::shared_ptr<dataType> dataPtr(std::make_shared<dataType>(dataQueue.front()));
        dataQueue.pop();
        return dataPtr;
    }
}