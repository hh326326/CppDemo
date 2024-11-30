#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "ThreadPool.h"

#include <iostream>

using std::cout;
using std::endl;

ThreadPool *gpool = nullptr;
class MyTask {
public:
    MyTask(const string &msg, const TcpConnectionPtr & con)
    : _msg(msg)
    , _con(con)
    {

    }
    void process() {
        // msg;
        // 在此处进行业务逻辑的处理decode、compute、encode
        for (auto & c: _msg) {
            c = toupper(c);
        }
        // msg1;
        // 然后在处理好业务逻辑之后，线程池需要将处理好
        // 之后的数据再发给Reactor/EventLoop
        // 然后通过Reactor/EventLoop将数据发给客户端
        //
        // 如果Reactor/EventLoop将数据打包成MyTask之后，通过
        // addTask交给了线程池，然后Reactor应该去处理自己的事情，
        // 同时线程池中的线程应该处理Reactor交给线程池处理
        // 的任务，也就是Reactor与线程池各自干自己的事情。然后
        // 当线程池处理好业务逻辑之后，应该主动通知Reactor将线程池处理
        // 好之后的数据接走
        //
        // 接下来的问题就是线程池与Reactor/EventLoop两类线程如何
        // 进行通信？也就是线程间如何通信？
        // 进程或者线程之间如何进行通信的方法eventfd如何使用？
        _con->sendInLoop(_msg);
    }
private:
    string _msg;
    TcpConnectionPtr _con;
};

//连接建立
void onNewConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has connected!" << endl;
}

//文件描述符可读(消息到达)
void onMessage(const TcpConnectionPtr &con)
{
    string msg = con->receive();
    cout << ">>recv client msg = " << msg << endl;
    //做业务逻辑的处理
    // 在此处，需要将接收到的数据msg打包给线程池处理
    MyTask task(msg, con);
    gpool->addTask(std::bind(&MyTask::process, task));
}

//连接断开
void onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!" << endl;
}

void test() {
    ThreadPool pool(4, 10);
    pool.start();
    gpool = &pool;

    TcpServer server("127.0.0.1", 8888);
    server.setAllCallback(std::move(onNewConnection),
                          std::move(onMessage),
                          std::move(onClose));
    server.start();
    server.stop();
}

int main(int argc, char *argv[])
{
    test();
    return 0;
}

