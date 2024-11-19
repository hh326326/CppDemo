#include "Thread.h"
#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

Thread::Thread()
: _thid(0)
, _isRunning(false)
{

}

Thread::~Thread()
{

}

//线程的启动
void Thread::start()
{
    //shift + k
    //为了消除threadFunc的this问题，需要将其设置为static
    /* int ret = pthread_create(&_thid, nullptr, threadFunc, nullptr); */
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create");
        return;
    }

    _isRunning = true;
}

//线程的停止
void Thread::stop()
{
    if(_isRunning)
    {
        int ret = pthread_join(_thid, nullptr);
        if(ret)
        {
            perror("pthread_join");
            return;
        }

        _isRunning = false;
    }
}

//线程入口函数
void *Thread::threadFunc(void *arg)
{
    //pth = arg = new WorkThread()
    Thread *pth = static_cast<Thread *>(arg);
    //Thread * this
    if(pth)
    {
        pth->run();//会体现多态
    }
    else
    {
        cout << "nullptr == pth" << endl;
    }

    /* return nullptr; */
    pthread_exit(nullptr);
}

