#ifndef __Consumer_H__
#define __Consumer_H__

#include "Thread.h"

class TaskQueue;

class Consumer
: public Thread
{
public:
    Consumer(TaskQueue &taskQue); 
    ~Consumer(); 
    
    void run() override;
private:
    TaskQueue & _taskQue;
};




#endif
