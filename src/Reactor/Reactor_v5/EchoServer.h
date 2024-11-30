#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "ThreadPool.h"
#include "TcpServer.h"

class MyTask
{
    public:
    MyTask(const string &msg, const TcpConnectionPtr &con);
    void process();

    private:
    string _msg;
    TcpConnectionPtr _con;
};

class EchoServer
{
    public:
        EchoServer(size_t threadNum, size_t queSize,
                   const string &ip, unsigned short port);
        ~EchoServer();

        void start();
        void stop();
    private:
        void onNewConnection(const TcpConnectionPtr &con);
        void onMessage(const TcpConnectionPtr &con);
        void onClose(const TcpConnectionPtr &con);

    private:
        ThreadPool _pool;
        TcpServer _server;
};

#endif