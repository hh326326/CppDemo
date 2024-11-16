#ifndef __Thread_H__
#define __Thread_H__

#include <pthread.h>

class Thread {
public:
    Thread();
    virtual ~Thread();

    void start(); // 线程的启动
    void stop(); // 线程的停止
private:
    // 不写static,第一个参数就为this指针
    static void * threadFunc(void* arg); // 线程入口函数
    virtual void run() = 0; // 抽象任务函数(线程需要执行的任务)

private:
    pthread_t _tid; // 线程id
    bool _isRunning; // 标识线程是否运行
};



#endif
