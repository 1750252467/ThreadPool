/*  File Name      tcp_connection.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 21:40
 *  Last Modified  2022-08-04 21:01
 */
#include "../../include/Reactor/tcp_connection.h"

#include <sstream>

#include "../../include/Reactor/event_loop.h"

using std::ostringstream;

namespace search_engine {

TcpConnection::TcpConnection(int fd, EventLoop* loop)
: _loop(loop),
  _sock(fd),
  _sockIO(fd),
  _localAddr(getLocalAddr()),
  _peerAddr(getPeerAddr())     {   }

void TcpConnection::send(const string& msg) {
    _sockIO.writeN(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const string& msg) {
    if (_loop) {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}

string TcpConnection::receive() {
    char buff[65535] = {0};
    _sockIO.readLine(buff, sizeof(buff));

    return string(buff);
}

string TcpConnection::toString() {
    ostringstream oss;
    oss << "[" << _localAddr.ip() << ": "
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ": "
        << _peerAddr.port() << "]";

    return oss.str();
}

bool TcpConnection::isClosed() const {
    char buf[10] = {0};
    int ret = recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return ret == 0;
}

void TcpConnection::handleConnectionCallback() {
    //this ->TcpConnection > (this - >shared_ptr)
    if (_onConectionCb) {
        /* _onConectionCb(shared_ptr<TcpConnection>(this)); */
        _onConectionCb(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback() {
    if (_onMessageCb) {
        _onMessageCb(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback() {
    if (_onCloseCb) {
        _onCloseCb(shared_from_this());
    }
}

//获取本端的网络地址信息
InetAddress TcpConnection::getLocalAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );

    int ret = getsockname(_sock.fd(), (struct sockaddr*)&addr, &len);
    if (-1 == ret) {
        perror("getsockname");
    }

    return InetAddress(addr);
}

//获取对端的网络地址信息
InetAddress TcpConnection::getPeerAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );

    int ret = getpeername(_sock.fd(), (struct sockaddr*)&addr, &len);
    if (-1 == ret) {
        perror("getpeername");
    }

    return InetAddress(addr);
}

} // namespace search_engine
