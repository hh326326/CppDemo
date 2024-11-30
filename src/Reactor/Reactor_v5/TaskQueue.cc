#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true)
{

}

TaskQueue::~TaskQueue()
{

}

//添加任务
void TaskQueue::push(ElemType &&task)
{
    //1、上锁
    MutexLockGuard autoLock(_mutex);

    //2、是不是满
    while(full())
    {
        //2.1、如果队列是满的，生产者需要睡眠
        _notFull.wait();
    }
    //2.2、如果队列不为满才能进行push操作
    _que.push(std::move(task));
    //并且将消费者唤醒
    _notEmpty.notify();
}

//获取任务
ElemType TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);

    while(empty() && _flag)
    {
        _notEmpty.wait();
    }

    if(_flag)
    {
        ElemType tmp = _que.front();
        _que.pop();
        _notFull.notify();

        return tmp;
    }
    else
    {
        return nullptr;
    }

}

//任务队列是空与满
bool TaskQueue::full() const
{
    return _queSize == _que.size();
}

bool TaskQueue::empty() const
{
    return 0 == _que.size();
}

//将所有等待在_notEmpty条件变量上的线程唤醒
void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}
