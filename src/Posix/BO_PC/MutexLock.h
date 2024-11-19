#ifndef __MutexLock_H__
#define __MutexLock_H__

#include "NonCopyable.h"
#include <pthread.h>

class MutexLock
: NonCopyable
{
public:
    MutexLock(); 
    ~MutexLock(); 

    // 上锁
    void lock();
    // 尝试上锁
    void trylock();
    // 解锁
    void unlock();
    // 获取数据成员
    pthread_mutex_t * getMutexLockPtr();
private:
    pthread_mutex_t _mutex;
};

class MutexLockGuard {
public:
    MutexLockGuard(MutexLock &mutex) 
    : _mutex(mutex)
    {
        _mutex.lock();
    }
    ~MutexLockGuard() {
        _mutex.unlock();
    }
private:
    MutexLock & _mutex;
};



#endif
