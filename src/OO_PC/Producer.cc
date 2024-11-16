#include "Producer.h"
#include "TaskQueue.h"

/* #include <stdlib.h> */
/* #include <time.h> */
#include <unistd.h>
#include <random>
#include <chrono>
#include <iostream>

Producer::Producer(TaskQueue &taskQue)
: _taskQue(taskQue)
{}

Producer::~Producer() 
{} 

void Producer::run(){
    // C
    /* ::srand(::clock()); */ // 种随机数种子
    /* int num = ::rand() % 1000; */ // 产生随机数

    // cpp
    // 使用系统时间的毫秒部分作为种子
    auto now = chrono::high_resolution_clock::now();
    auto seed = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    // 创建随机数生成器（梅森旋转算法）
    mt19937 gen(seed);
    // 创建一个范围为 1 到 1000 的均匀分布
    uniform_int_distribution<> distrib(1, 1000);

    // 生成随机数
    int cnt = 20;
    while(cnt--) {
        int num = distrib(gen);
        _taskQue.push(num);
        cout << ">>Producer produce number = " << num << endl;
        sleep(1);
    }
}
