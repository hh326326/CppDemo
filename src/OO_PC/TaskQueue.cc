#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize) 
: _queSize(queSize)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
{} 

 TaskQueue::~TaskQueue()
{} 

// 添加任务
void TaskQueue::push(const int &value) {
    // 1.上锁
    _mutex.lock();
    // 2.是不是满
    if(full()) {
        // 2.1 如果队列是满的，生产者需要睡眠
        _notFull.wait();
    }
    // 2.2 如果队列不为满才能进行push操作
    // 并且将消费者唤醒
    _que.push(value);
    _notEmpty.notify();

    // 3.解锁
    _mutex.unlock();
}

// 获取任务
int TaskQueue::pop() {
    // 1.上锁
    _mutex.lock();
    // 2.是不是空
    if(empty()) {
        // 2.1 如果为空，消费者需要睡觉
        _notEmpty.wait();
    }
    // 2.2 不为空则消费数据
    int tmp = _que.front(); 
    _que.pop();
    // 并且将生产者唤醒
    _notFull.notify();
    // 解锁
    _mutex.unlock();

    return tmp;
}

// 判断任务队列空与满
bool TaskQueue::full() const {
    return _queSize == _que.size();
}
bool TaskQueue::empty() const {
    return _que.size() == 0;
}
