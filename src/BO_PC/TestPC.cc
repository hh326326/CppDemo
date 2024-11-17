#include "Thread.h"
#include "Producer.h"
#include "Consumer.h"
#include "TaskQueue.h"

#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;

void test() {
    TaskQueue taskQue(10);
    Producer producer;
    Consumer consumer;
    unique_ptr<Thread> pro(new Thread(bind(&Producer::produce, &producer, std::ref(taskQue))));
    unique_ptr<Thread> con(new Thread(bind(&Consumer::consume, &consumer, std::ref(taskQue))));

    pro->start();
    con->start();

    pro->stop();
    con->stop();
}


int main()
{
    test();
    return 0;
}

