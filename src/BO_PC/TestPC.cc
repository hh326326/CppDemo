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
    Producer producer(taskQue);
    Consumer consumer(taskQue);
    unique_ptr<Thread> pro(new Thread(bind(&Producer::process, &producer)));
    unique_ptr<Thread> con(new Thread(bind(&Consumer::process, &consumer)));

    pro->start();
    con->start();

    pro->stop();
    con->stop();
}

void test2() {
    MutexLock mutex1;
    /* MutexLock mutex2 = mutex1; // error */

    MutexLock mutex3;
    /* mutex3 = mutex1; // error */
}

int main()
{
    test();
    return 0;
}

