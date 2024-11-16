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
    unique_ptr<Thread> pro(new Producer(taskQue));
    unique_ptr<Thread> con(new Consumer(taskQue));

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

