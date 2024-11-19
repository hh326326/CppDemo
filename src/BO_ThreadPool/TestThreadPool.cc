#include "ThreadPool.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using std::cout;
using std::endl;

class MyTask
{
public:
    void process() 
    {
        ::srand(::clock());//种随机种子
        int number = ::rand() % 100;//产生随机数
        cout << ">>Bo_ThreadPool: MyTask number = " << number << endl;
    }
};

void test()
{
    MyTask task;
    ThreadPool pool(4, 10);
    pool.start();

    int cnt = 20;
    while(cnt--)
    {
        //添加任务
        pool.addTask(std::bind(&MyTask::process, &task));
        cout << "cnt = " << cnt << endl;
    }

    pool.stop();
}

int main(int argc, char *argv[])
{
    test();
    return 0;
}

