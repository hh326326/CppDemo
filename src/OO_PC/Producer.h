#ifndef __Producer_H__
#define __Producer_H__

#include "Thread.h"

class TaskQueue;

class Producer
: public Thread
{
public:
    Producer(TaskQueue &taskQue); 
    ~Producer(); 
    
    void run() override;
private:
    TaskQueue & _taskQue;
};




#endif
