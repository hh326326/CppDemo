#include "Producer.h"
#include "Consumer.h"
#include "TaskQueue.h"

#include <iostream>
#include <memory>
#include <thread>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::thread;

void test() {
    TaskQueue taskQue(10);

    Producer producer;
    Consumer consumer;

    thread pro(&Producer::produce, &producer, std::ref(taskQue));
    thread con(&Consumer::consume, &consumer, std::ref(taskQue));

    pro.join();
    con.join();
}


int main()
{
    test();
    return 0;
}

