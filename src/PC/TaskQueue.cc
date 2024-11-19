#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notFull()
, _notEmpty()
{

}

TaskQueue::~TaskQueue()
{

}

//添加任务
void TaskQueue::push(const int &value)
{
    unique_lock<mutex> autoLock(_mutex);

    while(full())
    {
        _notFull.wait(autoLock);
    }
    _que.push(value);
    _notEmpty.notify_one();
}

//获取任务
int TaskQueue::pop()
{
    unique_lock<mutex> autoLock(_mutex);

    while(empty())
    {
        _notEmpty.wait(autoLock);
    }

    int tmp = _que.front();
    _que.pop();
    _notFull.notify_one();

    return tmp;
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
