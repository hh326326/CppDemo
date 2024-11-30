#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <iostream>

using std::cout;
using std::endl;
using std::cerr;

EventLoop::EventLoop(Acceptor &acceptor)
: _epfd(createEpollFd())
, _evtList(1024)
, _isLooping(false)
, _acceptor(acceptor)
, _evtfd(createEventFd()) // 创建用于通信的文件描述符
, _mutex()
{
    //将listenfd放在红黑树上进行监听（socket）
    int listenfd = _acceptor.fd();
    addEpollReadFd(listenfd);

    // 将用于通信的文件描述符放在红黑树上进行监听
    addEpollReadFd(_evtfd);
}

EventLoop::~EventLoop()
{
    close(_epfd);
    close(_evtfd);
}

//循环与否
void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

//封装类epoll_wait函数
void EventLoop::waitEpollFd()
{
    int nready = 0;
    do
    {
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 3000);
    }while(-1 == nready && errno == EINTR);

    if(-1 == nready)
    {
        cerr << "-1 == nready" << endl;
        return;
    }
    else if(0 == nready)
    {
        cout << ">>epoll_wait timeout" << endl;
    }
    else
    {
        //可以判断一下，文件描述符是不是已经达到了1024
        //如果达到1024就需要进行扩容
        if(nready == (int)_evtList.size())
        {
            _evtList.reserve(2 * nready);
        }

        for(int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            //查看文件描述符是不是listenfd
            if(fd == _acceptor.fd())
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    //处理新的连接
                    handleNewConnection();
                }
            }
            // 就绪的文件描述符是用于通信的文件描述符
            else if (fd == _evtfd) {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleRead();
                    // 执行所有的待执行的"任务"
                    doPendingFunctors();
                }
            }
            else
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    //处理老的连接
                    handleMessage(fd);
                }
            }
        }
    }
}

//处理新的连接
void EventLoop::handleNewConnection()
{
    int connfd = _acceptor.accept();
    if(connfd < 0)
    {
        perror("handleNewConnection accept");
        return;
    }

    addEpollReadFd(connfd);

    //就表明三次握手已经建立成功了
    /* TcpConnection con(connfd); */
    TcpConnectionPtr con(new TcpConnection(connfd, this));

    //将三个回调函数注册给TcpConnection
    con->setNewConnectionCallback(_onNewConnectionCb);//连接建立的注册
    con->setMessageCallback(_onMessageCb);//消息到达的注册
    con->setCloseCallback(_onCloseCb);//连接断开的注册

    //以键值对的形式存起来
    /* _conns.insert(std::make_pair(connfd, con)); */
    _conns[connfd] = con;

    con->handleNewConnectionCallback();//连接建立的事件触发的时机已经到达
}

//处理老的连接上的消息
void EventLoop::handleMessage(int fd)
{
    auto it = _conns.find(fd);
    if(it != _conns.end())
    {
        //连接是存在的，可以进行数据的收发
        //read/recv = ret = 0
        bool flag = it->second->isClosed();//读的时候客户端是不是与服务器断开
        if(flag)
        {
            //连接已经断开
            it->second->handleCloseCallback();//连接断开的事件的触发时机已经到达
            delEpollReadFd(fd);//将文件描述符从红黑树上摘除掉
            _conns.erase(it);//同时将该链接从map中删除
        }
        else
        {
            it->second->handleMessageCallback();//消息到达事件的触发时机已经到达
        }
    }
    else
    {
        //连接不存在，可以直接让程序退出来
        cout << "连接不存在" << endl;
        return;
    }

}

//epfd的创建
int EventLoop::createEpollFd()
{
    int fd = ::epoll_create(100);
    if(fd < 0)
    {
        perror("epoll_create");
        return fd;
    }
    return fd;
}

//监听文件描述符
void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret < 0)
    {
        perror("epoll_ctl add");
        return;
    }
}

//取消文件描述符的监听
void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret < 0)
    {
        perror("epoll_ctl del");
        return;
    }
}

void EventLoop::setNewConnectionCallback(TcpConnectionCallback &&cb)
{
    _onNewConnectionCb = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onCloseCb = std::move(cb);
}

void EventLoop::handleRead() {
     uint64_t two = 1;
    ssize_t ret = read(_evtfd, &two, sizeof(uint64_t));
    if (ret != sizeof(uint64_t)) {
        perror("read");
        return;
    }
}

// 将存放在vector中的"任务"进行遍历执行
// "任务"就是线程池处理好之后的数据以及发送能力TcpConnection中
// 的send函数
void EventLoop::doPendingFunctors() {
    // 将锁的粒度变得更小，提高并发程度
    vector<Functor> tmp;
    {
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pendings);
    }

    for (auto &cb : tmp)
    {
        cb(); // 回调 "任务"
   } 
}

int EventLoop::createEventFd() {
    int fd = eventfd(10, 0);
    if (fd < 0) {
        perror("eventfd");
        return -1;
    }
    return fd;
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t ret = write(_evtfd, &one, sizeof(uint64_t));
    if (ret != sizeof(uint64_t))
    {
        perror("write");
        return;
    }
}

void EventLoop::runInLoop(Functor &&cb) {
    {
        // 锁的粒度 (锁的范围)
        MutexLockGuard autoLock(_mutex);
        _pendings.push_back(std::move(cb));
    }

    // 只要将任务传递给EventLoop/Reactor之后
    // 就应该通知EventLoop/Reactor
    wakeup();
}