#ifndef __Producer_H__
#define __Producer_H__


class TaskQueue;

class Producer
{
public:
    Producer(); 
    ~Producer(); 

    void produce(TaskQueue &taskQue);    
};




#endif
