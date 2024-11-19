#ifndef __THREAD_H__
#define __THREAD_H__

#include "NonCopyable.h"
#include <pthread.h>

class Thread
: NonCopyable
{
public:
    Thread();
    virtual ~Thread();

    //线程的启动
    void start();
    //线程的停止
    void stop();

private:
    //线程入口函数
    static void *threadFunc(void *arg);
    //抽象的任务(线程需要执行的任务)
    virtual void run() = 0;
private:
    pthread_t _thid;//线程id
    bool _isRunning;//标识线程是否运行的标志
};


#endif
