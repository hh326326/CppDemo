#ifndef __MutexLock_H__
#define __MutexLock_H__

#include <pthread.h>

class MutexLock
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





#endif
