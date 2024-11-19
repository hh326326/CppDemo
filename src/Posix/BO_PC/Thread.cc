#include "Thread.h"
#include <cstdio>
#include <iostream>

using namespace std;

Thread::Thread(const TheadCallback &cb) 
: _tid(0)
, _isRunning(false)
, _cb(std::move(cb)) // 注册
{}

Thread::~Thread() {}

// 线程的启动
void Thread::start() {
    // 数字 + shift + k 跳到man的几号手册的这个函数
    // 为了消除threadFunc的this指针问题，需要将其设置为static
    /* int ret = pthread_create(&_tid, nullptr, threadFunc, nullptr); */
    int ret = pthread_create(&_tid, nullptr, threadFunc, this);
    if(ret) {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
} 

// 线程的停止
void Thread::stop() {
    if(_isRunning) {
        int ret = pthread_join(_tid, nullptr);
        if(ret) {
            perror("pthread_join");
            return;
        }
        _isRunning = false;
    }
}

// 线程入口函数
void * Thread::threadFunc(void* arg) {
    /* Thread * this */
    /* this->run(); */
    // pth =  arg = new MyThread()
    Thread * pth = static_cast<Thread *>(arg);
    if(pth) {
        pth->_cb(); // 回调
    }
    else {
        cout << "nullptr == pth" << endl;
    }
    /* return nullptr; */
    pthread_exit(nullptr);
} 

