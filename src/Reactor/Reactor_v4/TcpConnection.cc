#include "TcpConnection.h"
#include "EventLoop.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
: _loop(loop)
, _sockIO(fd)
, _sock(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
{

}

TcpConnection::~TcpConnection()
{ }

void TcpConnection::send(const string &msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

// 此处的sendInLoop中的形参msg是线程池处理好
// 之后的，可以发给Reactor/EventLoop的数据, 但是对于
// EventLoop/Reactor而言，即使拿到处理好之后的数据
// 也不能将数据发生给客户端，因为对于EventLoop/Reactor
// 而言，它本身没有发送数据的能力
// 真正在整个设计中，具备发送数据能力的类有两个
// 一个是TcpConnection另一个是SocketIO, 而SocketIO
// 更偏向底层，真正对外发送数据的就是
// TcpConnection对象，或者更具体的说，
// 就是TcpConnection中的send函数
// 所以需要将发送数据能力的TcpConnection中的
// send 函数，以及要发送的数据msg一起打包成
// "任务", 交给EventLoop/Reactor, 这样EventLoop
// Reactor就可以将数据发送给客户端
void TcpConnection::sendInLoop(const string &msg) {
    // function<void()> f = bind(&TcpConnection::send, this, msg);
    if (_loop) {
        // function<void()> &&cb
        // void runInLoop(function<void()> &&cb)
        _loop->runInLoop(bind(&TcpConnection::send, this, msg));
    }
}

string TcpConnection::receive()
{
    char buff[65535] = {0};
    _sockIO.readLine(buff, sizeof(buff));

    return string(buff);
}

//可以通过该函数判断读的数据是不是空的，也就是有没有断开
bool TcpConnection::isClosed() const
{
    char buf[10]= {0};
    int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return (0 == ret);
}

string TcpConnection::toString()
{
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();
}

//获取本端的网络地址信息
InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    //获取本端地址的函数getsockname
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }

    return InetAddress(addr);
}

//获取对端的网络地址信息
InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    //获取对端地址的函数getpeername
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername");
    }

    return InetAddress(addr);
}

void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback &cb)
{
    _onNewConnectionCb = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    _onMessageCb = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    _onCloseCb = cb;
}

//三个回调的执行
void TcpConnection::handleNewConnectionCallback()
{
    if(_onNewConnectionCb)
    {
        /* _onNewConnectionCb(shared_ptr<TcpConnection>(this)); */
        _onNewConnectionCb(shared_from_this());
    }
    else
    {
        cout << "_onNewConnectionCb == nullptr" << endl;
    }
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessageCb)
    {
        _onMessageCb(shared_from_this());
    }
    else
    {
        cout << "_onMessageCb == nullptr" << endl;
    }
}

void TcpConnection::handleCloseCallback()
{
    if(_onCloseCb)
    {
        _onCloseCb(shared_from_this());
    }
    else
    {
        cout << "_onCloseCb == nullptr" << endl;
    }
}
