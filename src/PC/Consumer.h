#ifndef __Consumer_H__
#define __Consumer_H__

class TaskQueue;

class Consumer
{
public:
    Consumer(); 
    ~Consumer(); 
    
    void consume(TaskQueue &taskQue);
};




#endif
