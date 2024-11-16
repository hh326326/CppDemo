#ifndef __Consumer_H__
#define __Consumer_H__

#include "Thread.h"

class TaskQueue;

class Consumer
{
public:
    Consumer(TaskQueue &taskQue); 
    ~Consumer(); 
    
    void process();
private:
    TaskQueue & _taskQue;
};




#endif
