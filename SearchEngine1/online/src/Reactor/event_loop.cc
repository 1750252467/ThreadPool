/*  File Name      event_loop.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 09:22
 *  Last Modified  2022-08-03 12:22
 */
#include "../../include/Reactor/event_loop.h"

#include <unistd.h>
#include <sys/eventfd.h>

#include <iostream>

#include "../../include/Reactor/acceptor.h"

namespace search_engine {

EventLoop::EventLoop(Acceptor& acceptor)
    : _epfd(createEpollFd()),
      _evfd(createEventFd()),
      _acceptor(acceptor), 
      _isLooping(false),
      _evtList(1024)      {
        addEpollReadFd(_acceptor.fd());  
        addEpollReadFd(_evfd);           
    }

EventLoop::~EventLoop() {
    if (_epfd)   
        close(_epfd);
    if (_evfd)   
        close(_evfd);
}

void EventLoop::loop() {
    _isLooping = true;
    while (_isLooping)   
        waitEpollFd();
}

inline
void EventLoop::unloop() {
    _isLooping = false;
}

void EventLoop::runInLoop(Functor&& cb) {
    {
        MutexLockGuard autoLock(_mutex);
        _pendingsCb.push_back(std::move(cb));
    }

    wakeup();
}

void EventLoop::waitEpollFd() {
    int nready = -1;
    do {
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 3000);
    } while (-1 == nready && errno == EINTR);

    if(-1 == nready) {
        perror("epoll_wait nready == -1");
        return;
    } else if (0 == nready) {
        printf("\e[0;31m" "***epoll_wait timeout***\n" "\e[0m");
    } else {

        if (nready == (int)_evtList.size()) {
            _evtList.resize(2 * nready);
        }

        for (int idx = 0; idx < nready; ++idx) {
            int fd = _evtList[idx].data.fd;

            if (fd == _acceptor.fd()) {            
                if (_evtList[idx].events & EPOLLIN) {
                    handleNewConnection();
                }

            } else if (fd == _evfd) {   
                if (_evtList[idx].events & EPOLLIN) {
                    //EventLoop::runInloop()中wakeup唤醒
                    handleRead();
                    doPendingFunctors();
                }

            } else {
                if(_evtList[idx].events & EPOLLIN) {
                    handleMessage(fd);
                }
            }
        }
    }
}


void EventLoop::handleNewConnection() {
    int peerfd = _acceptor.accept();
    if(peerfd < 0) {
        perror("peerfd");
    }

    addEpollReadFd(peerfd);

    TcpConnectionPtr con(new TcpConnection(peerfd, this));

    con->setConnectionCallback(_onConectionCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    _conns.insert(std::make_pair(peerfd, con));
    con->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd) {
    map<int, TcpConnectionPtr>::iterator it = _conns.find(fd);
    if (it != _conns.end()) {
        bool flag = it->second->isClosed();
        if (flag) {
            it->second->handleCloseCallback();
            delEpollReadFd(fd);
            _conns.erase(it);
        }
        else {
            it->second->handleMessageCallback();
        }
    } else {
        std::cout << "该连接不存在" << std::endl;
    }
}

//执行所有的任务
void EventLoop::doPendingFunctors()
{
    vector<Functor> tmp;   
    {
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pendingsCb);            
    }

    for (function<void()>& cb : tmp) {
        cb();
    }
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    int ret = read(_evfd, &one, sizeof(one));
    if (ret != sizeof(one)) {
        perror("read");
        return;
    }
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    int ret = write(_evfd, &one, sizeof(one));
    if (ret != sizeof(one)) {
        perror("read");
        return;
    }
}

int EventLoop::createEventFd() {
    int fd = eventfd(0, 0);
    if (fd < 0) {
        perror("eventfd");
        return -1;
    }
    return fd;
}

int EventLoop::createEpollFd() {
    int fd = epoll_create1(0);
    if (fd < 0) {
        perror("epoll_create1");
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd) {
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if (ret < 0) {
        perror("epoll_ctl add");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd) { 
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if (ret < 0) {
        perror("epoll_ctl del");
        return;
    }
}

} // namespace search_engine
