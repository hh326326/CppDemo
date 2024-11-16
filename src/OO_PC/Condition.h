#ifndef __Condition_H__
#define __Condition_H__

/* #include "MutexLock.h" */
#include <pthread.h>

class MutexLock;

class Condition
{
public:
    Condition(MutexLock &mutex); 
    ~Condition(); 

    // 睡眠
    void wait();
    // 通知
    void notify();
    void notifyAll();
private:
    MutexLock & _mutex;
    pthread_cond_t _cond;
};




#endif
