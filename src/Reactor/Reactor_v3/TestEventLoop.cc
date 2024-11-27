#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include "TcpServer.h"

#include <iostream>

using std::cout;
using std::endl;

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
    //...
    //...
    //处理完成之后，讲结果返回给客户端
    con->send(msg);
}

//连接断开
void onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!" << endl;
}

void test() {
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

