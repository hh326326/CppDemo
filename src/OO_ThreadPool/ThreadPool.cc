#include "ThreadPool.h"
#include "Task.h"

#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{
    //预留空间
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool()
{

}

//线程池的启动与停止
void ThreadPool::start()
{
    //创建工作线程
    for(size_t idx = 0; idx != _threadNum; ++idx)
    {
        _threads.push_back(thread(&ThreadPool::doTask, this));
    }

}

void ThreadPool::stop()
{
    // 任务执行不完就不能让子线程退出
    while(!_taskQue.empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    _isExit = true;

    // 在回收之前将所有的工作线程唤醒
    _taskQue.wakeup();
    //将工作线程停止
    for(auto &th : _threads)
    {
        th.join();
    }
}

//添加任务与获取任务
void ThreadPool::addTask(Task *ptask)
{
    if(ptask)
    {
        _taskQue.push(ptask);
    }
}

Task *ThreadPool::getTask()
{
    return _taskQue.pop();
}

//线程池交给工作线程执行的任务
void ThreadPool::doTask()
{
    //用的是线程池是否退出的标志位???
    //子线程（工作线程）在获取完任务之后，那么任务队列就为空
    //接下来子线程会执行任务，也就是执行process函数，而主线程
    //会执行stop函数中的将标志位_isExit设置为true的操作。如果
    //子线程执行process的速率非常慢，那么当子线程执行任务之前
    //主线程已经将标志位设置为了true，那么子线程就不会进入到
    //while循环中，那么程序就可以正常结束；如果子线程执行任务
    //process的速率非常的快，但是主线程还没有将标志位设置为
    //true，那么子线程执行完process后，又进入了while循环，在
    //执行getTask的时候，也就是会执行TaskQueue中的pop，然后
    //睡眠
    while(!_isExit)
    {
        Task *ptask = getTask();
        if(ptask)
        {
            ptask->process();//执行任务
        }
        else
        {
            cout << "nullptr == ptask" << endl;
        }
    }
}
