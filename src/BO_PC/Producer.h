#ifndef __Producer_H__
#define __Producer_H__

#include "Thread.h"

class TaskQueue;

class Producer
{
public:
    Producer(); 
    ~Producer(); 

    void produce(TaskQueue &taskQue);    
};




#endif
