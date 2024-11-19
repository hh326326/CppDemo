#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class Thread;
class Task;

class ThreadPool
{
    //因为WorkThread要访问ThreadPool的
    //私有成员doTask函数
    friend class WorkThread;
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    //线程池的启动与停止
    void start();
    void stop();

    //添加任务与获取任务
    void addTask(Task *ptask);
private:
    Task *getTask();
    //线程池交给工作线程执行的任务
    void doTask();

private:
    size_t _threadNum;//子线程的数目
    size_t _queSize;//任务队列的大小
    vector<unique_ptr<Thread>> _threads;//存放工作线程的容器
    TaskQueue _taskQue;//存放任务的容器
    bool _isExit;//标志线程池是否结束的标志位
};

#endif
