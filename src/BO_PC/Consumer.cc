#include "Consumer.h"
#include "TaskQueue.h"

#include <unistd.h>
#include <iostream>

Consumer::Consumer(TaskQueue &taskQue)
: _taskQue(taskQue)
{}

Consumer::~Consumer() 
{} 

void Consumer::process(){
    int cnt = 20;
    while(cnt--) {
        int number = _taskQue.pop();
        cout << "<<Consumer consume number = " << number << endl;
        sleep(1);
    }
}
