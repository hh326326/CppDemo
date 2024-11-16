#ifndef __TaskQueue_H__
#define __TaskQueue_H__

#include "Condition.h"
#include "MutexLock.h"
#include <queue>

using namespace std;

class TaskQueue
{
public:
    TaskQueue(size_t queSize); 
    ~TaskQueue(); 
    
    // 添加任务
    void push(const int &value);
    // 获取任务
    int pop();

    // 判断任务队列空与满
    bool full() const;
    bool empty() const;
private:
    size_t _queSize; // 任务队列大小
    queue<int> _que; // 存放任务的数据结构
    MutexLock _mutex; // 互斥锁
    Condition _notFull; // 非满条件变量 (生产者)
    Condition _notEmpty; // 非空条件变量 (消费者)
};





#endif
