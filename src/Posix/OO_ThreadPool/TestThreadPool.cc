#include "Task.h"
#include "ThreadPool.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using std::cout;
using std::endl;

class MyTask
: public Task
{
public:
    void process() override
    {
        ::srand(::clock());//种随机种子
        int number = ::rand() % 100;//产生随机数
        cout << ">>MyTask number = " << number << endl;
    }
};

// 两个问题
// 1. 任务执行不完线程池就退出来了

// 2.任务执行完毕但是线程池卡住，无法退出
void test()
{
    unique_ptr<Task> ptask(new MyTask());
    ThreadPool pool(4, 10);
    pool.start();

    int cnt = 20;
    while(cnt--)
    {
        //添加任务
        pool.addTask(ptask.get());
        cout << "cnt = " << cnt << endl;
    }

    pool.stop();
}

int main(int argc, char *argv[])
{
    test();
    return 0;
}

