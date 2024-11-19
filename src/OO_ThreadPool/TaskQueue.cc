#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _flag(true)
{

}

TaskQueue::~TaskQueue()
{

}

//添加任务
void TaskQueue::push(ElemType ptask)
{
    unique_lock<mutex> autoLock(_mutex);

    while(full())
    {
        _notFull.wait(autoLock);
    }
    _que.push(ptask);
    _notEmpty.notify_one();
}

//获取任务
ElemType TaskQueue::pop()
{
    unique_lock<mutex> autoLock(_mutex);

    while(empty() && _flag)
    {
        _notEmpty.wait(autoLock);
    }

    if(_flag)
    {
        ElemType tmp = _que.front();
        _que.pop();
        _notFull.notify_one();

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
    _notEmpty.notify_all();
}
